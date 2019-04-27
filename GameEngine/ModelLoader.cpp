#include "ModelLoader.h"


#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <fstream>
#include <sstream>

#include "RenderManager.h"

//Loads in a model
iModel * ModelLoader::LoadModel(const std::string & fileName)
{
	if (EndsWith(fileName, ".obj"))
	{
		return LoadOBJ(fileName);
	}
	else if (EndsWith(fileName, ".dae"))
	{
		return LoadDAE(fileName);
	}
	//TODO Log that the file does not exist
	return nullptr;
}

/*//May get rid of method need to change so that it also returns bool if the fileName has changed
void ModelLoader::SimpleFormatExists(const string & fileName, const string & fileType)
{
	string oldFileName = fileName;
	fileName.replace(fileName.size() - fileType.size(), fileType.size(), fileType);

	ifstream reader(fileName.c_str());
	bool fileOpen = reader.fail();
	reader.close();

	if (!fileOpen)
	{
		fileName = oldFileName;
	}
}*/

//Loads in OBJ model
StaticModel * ModelLoader::LoadOBJ(const std::string & fileName)
{
	std::ifstream reader(fileName.c_str());

	if (reader.fail())
	{
		//TODO: Log that the file does not exist
		reader.close();
		return nullptr;
	}

	std::vector<std::string> vertexString;
	std::vector<std::string> textureString;
	std::vector<std::string> normalString;
	std::vector<std::string> faceString;

	while (!reader.eof())
	{
		std::string line;
		reader >> line;

		if (line == "v")
		{
			getline(reader, line);
			vertexString.push_back(line);
		}
		else if (line == "vt")
		{
			getline(reader, line);
			textureString.push_back(line);
		}
		else if (line == "vn")
		{
			getline(reader, line);
			normalString.push_back(line);
		}
		else if (line == "f")
		{
			getline(reader, line);
			faceString.push_back(line);
		}
	}

	reader.close();

	std::vector<std::string> modelData;
	std::vector<int> indexes;

	for (int i = 0; i < faceString.size(); i++)
	{
		int vertex;
		int texture;
		int normal;
		char slash;

		std::istringstream line(faceString[i]);
		for (int j = 0; j < 3; j++)
		{
			line >> vertex >> slash >> texture >> slash >> normal;

			if (normal == 0)
			{
				//TODO: Log that the .obj file needs to include both normals and texture coordinates
				return nullptr;
			}

			const std::string lineToAdd = vertexString[vertex - 1] + " " + textureString[texture - 1] + " " + normalString[normal - 1];
			const int index = FindInVector(modelData, lineToAdd);
			if (index != -1)
			{
				indexes.push_back(index);
			}
			else
			{
				indexes.push_back(modelData.size());
				modelData.push_back(lineToAdd);
			}
		}
	}

	std::vector<glm::vec3> vertex;
	std::vector<glm::vec2> texture;
	std::vector<glm::vec3> normal;

	for (int i = 0; i < modelData.size(); i++)
	{
		float vertexX;
		float vertexY;
		float vertexZ;
		float textureX;
		float textureY;
		float normalX;
		float normalY;
		float normalZ;

		std::istringstream line(modelData[i]);
		line >> vertexX >> vertexY >> vertexZ >> textureX >> textureY >> normalX >> normalY >> normalZ;

		vertex.push_back(glm::vec3(vertexX, vertexY, vertexZ));
		texture.push_back(glm::vec2(textureX, textureY));
		normal.push_back(glm::vec3(normalX, normalY, normalZ));
	}

	std::vector<glm::vec3> tangents;
	/*vector<vec3> bitangents;*/

	TangentSpace(indexes, vertex, texture, tangents/*, bitangents*/);

	return RenderManager::Instance()->CreateStaticModel(vertex, texture, normal, indexes, tangents);
}


//Finds the same vector
int ModelLoader::FindInVector(std::vector<std::string> & list, const std::string & toFind)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] == toFind)
		{
			return i;
		}
	}
	return -1;
}

//Loads in collada file
AnimatedModel * ModelLoader::LoadDAE(const std::string & fileName)
{
	Assimp::Importer import;
	const aiScene * const scene = import.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return nullptr;
	}

	AnimatedModel * const animatedModel = new AnimatedModel();

	std::vector<Node *> nodes;

	std::vector<Bone *> bones;

	RecursiveNodeProcess(nodes, scene->mRootNode, nullptr);
	AnimNodeProcess(animatedModel, scene);

	const glm::mat4 globalInverseTransform = AiToGLMMat4(scene->mRootNode->mTransformation);

	ProcessNode(scene->mRootNode, scene, animatedModel);

	std::vector<Animation *> const animations = animatedModel->GetAnimations();

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++)
		{
			std::string boneName = scene->mMeshes[i]->mBones[j]->mName.data;
			glm::mat4 boneMat = AiToGLMMat4(scene->mMeshes[i]->mBones[j]->mOffsetMatrix);
			Node * node = FindNode(nodes, boneName);

			std::map<Animation *, AnimNode *> animNodes = std::map<Animation *, AnimNode *>();
			for (int k = 0; k < animations.size(); k++)
			{
				AnimNode * animNode = FindAiNodeAnim(animations.at(k)->GetAnimNodes(), boneName);
				animNodes.insert(std::pair<Animation *, AnimNode *>(animations.at(k), animNode));
			}

			Bone * const bone = new Bone(boneName, node, animNodes, boneMat);
			bones.push_back(bone);
		}
	}

	for (int i = 0; i < bones.size(); i++)
	{
		const std::string boneName = bones.at(i)->GetName();

		Node * const parent = FindNode(nodes, boneName)->GetParent();

		std::string parentName = "";

		if (parent)
		{
			parentName = parent->GetName();
		}

		Bone * const parentBone = FindBone(bones, parentName);

		bones.at(i)->SetParentBone(parentBone);
	}

	animatedModel->SetBones(bones);
	animatedModel->SetNodes(nodes);
	animatedModel->SetGlobalInverse(globalInverseTransform);
	
	return animatedModel;
}

//Processes the nodes in scene
void ModelLoader::ProcessNode(aiNode * const node, const aiScene * const scene, AnimatedModel * const animatedModel)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh * const mesh = scene->mMeshes[node->mMeshes[i]];
		animatedModel->AddMesh(ProcessMesh(node, mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, animatedModel);
	}
}

//Processes the mesh
AnimatedModelMesh * ModelLoader::ProcessMesh(const aiNode * const node, const aiMesh * const pMesh, const aiScene * const scene)
{
	std::vector<glm::vec3> vertexes;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;

	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		const aiVector3D aiVertex = pMesh->mVertices[i];
		const aiVector3D aiNormal = pMesh->mNormals[i];
		const aiVector3D aiTexture = pMesh->HasTextureCoords(0) ? pMesh->mTextureCoords[0][i] : aiVector3D(0);

		const glm::vec3 vertex = glm::vec3(aiVertex.x, aiVertex.y, aiVertex.z);
		const glm::vec3 normal = glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z);
		const glm::vec2 texture = glm::vec2(aiTexture.x, aiTexture.y);

		vertexes.push_back(vertex);
		normals.push_back(normal);
		textures.push_back(texture);
	}

	std::vector<int> indices;

	for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
	{
		const aiFace face = pMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	const int WEIGHTS_PER_VERTEX = 4;
	const int boneArraysSize = pMesh->mNumVertices * WEIGHTS_PER_VERTEX;

	std::vector<int> boneIDs;
	boneIDs.resize(boneArraysSize);

	std::vector<float>boneWeights;
	boneWeights.resize(boneArraysSize);

	std::vector<glm::ivec4> IDs;
	IDs.resize(pMesh->mNumVertices);

	std::vector<glm::vec4> Weights;
	Weights.resize(pMesh->mNumVertices);

	for (int i = 0; i < pMesh->mNumBones; i++)
	{
		aiBone * const aiBone = pMesh->mBones[i];

		for (int j = 0; j < aiBone->mNumWeights; j++)
		{
			const aiVertexWeight weight = aiBone->mWeights[j];

			const unsigned int vertexStart = weight.mVertexId * WEIGHTS_PER_VERTEX;

			for (int k = 0; k < WEIGHTS_PER_VERTEX; k++)
			{
				if (boneWeights.at(vertexStart + k) == 0)
				{
					boneWeights.at(vertexStart + k) = weight.mWeight;
					boneIDs.at(vertexStart + k) = i;

					IDs.at(weight.mVertexId)[k] = i;
					Weights.at(weight.mVertexId)[k] = weight.mWeight;

					break;
				}
			}
		}
	}

	return RenderManager::Instance()->CreateAnimatedModelMesh(vertexes, normals, textures, Weights, IDs, indices);
}

//Turns Assimp nodes, into game engine nodes
void ModelLoader::RecursiveNodeProcess(std::vector<Node*> & nodes, aiNode * const pNode, Node * const parentNode)
{
	std::string name = pNode->mName.data;
	glm::mat4 trans = AiToGLMMat4(pNode->mTransformation);
	Node * const node = new Node(name, trans);
	node->SetParent(parentNode);
	nodes.push_back(node);

	for (int i = 0; i < pNode->mNumChildren; i++)
	{
		RecursiveNodeProcess(nodes, pNode->mChildren[i], node);
	}
}

//Creates animations
void ModelLoader::AnimNodeProcess(AnimatedModel * const animationModel, const aiScene * const scene)
{
	if (scene->mNumAnimations == 0)
	{
		return;
	}

	for (int i = 0; i < scene->mNumAnimations; i++)
	{
		std::string name = scene->mAnimations[i]->mName.data;
		float startTime = 0;
		float endTime = scene->mAnimations[i]->mDuration / scene->mAnimations[i]->mTicksPerSecond;

		std::vector<AnimNode *> animNodes;

		for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
		{
			std::string animNodeName = scene->mAnimations[i]->mChannels[j]->mNodeName.data;
			std::vector<Vec3AnimKey *> positionKeys;
			std::vector<QuatAnimKey *> rotationKeys;
			std::vector<Vec3AnimKey *> scaleKeys;

			for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++)
			{
				float time = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;
				const aiVector3D aiValue = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue;
				glm::vec3 value(aiValue.x, aiValue.y, aiValue.z);

				Vec3AnimKey * const posKey = new Vec3AnimKey(time, value);

				positionKeys.push_back(posKey);
			}

			for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++)
			{
				float time = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime;
				const aiQuaternion aiValue = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue;
				glm::quat value(aiValue.w, aiValue.x, aiValue.y, aiValue.z);

				QuatAnimKey * const rotKey = new QuatAnimKey(time, value);

				rotationKeys.push_back(rotKey);
			}

			for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k++)
			{
				float time = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime;
				const aiVector3D aiValue = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue;
				glm::vec3 value(aiValue.x, aiValue.y, aiValue.z);

				Vec3AnimKey * const scaKey = new Vec3AnimKey(time, value);

				scaleKeys.push_back(scaKey);
			}

			AnimNode * const animNode = new AnimNode(animNodeName, positionKeys, rotationKeys, scaleKeys);
			animNodes.push_back(animNode);
		}

		Animation * const animation = new Animation(name, startTime, endTime, animNodes);

		animationModel->AddAnimation(animation);
	}
}

//Calculates tangent space
void ModelLoader::TangentSpace(std::vector<int> & indices, std::vector<glm::vec3> & vertex, std::vector<glm::vec2> & texture, std::vector<glm::vec3> & tangents/*, vector<vec3> & bitangents*/)
{
	std::vector<int> numTimesUsed;

	for (int i = 0; i < vertex.size(); i++)
	{
		numTimesUsed.push_back(0);
		tangents.push_back(glm::vec3(0));
		/*bitangents.push_back(vec3(0));*/
	}

	for (int i = 0; i < indices.size(); i += 3)
	{
		CalculateTangent(vertex[indices.at(i)], vertex[indices.at(i + 1)], vertex[indices.at(i + 2)],
			texture[indices.at(i)], texture[indices.at(i + 1)], texture[indices.at(i + 2)], tangents[indices.at(i)]/*, bitangents[indices.at(i)]*/, numTimesUsed[indices.at(i)]);
		CalculateTangent(vertex[indices.at(i)], vertex[indices.at(i + 1)], vertex[indices.at(i + 2)],
			texture[indices.at(i)], texture[indices.at(i + 1)], texture[indices.at(i + 2)], tangents[indices.at(i + 1)]/*, bitangents[indices.at(i + 1)]*/, numTimesUsed[indices.at(i + 1)]);
		CalculateTangent(vertex[indices.at(i)], vertex[indices.at(i + 1)], vertex[indices.at(i + 2)],
			texture[indices.at(i)], texture[indices.at(i + 1)], texture[indices.at(i + 2)], tangents[indices.at(i + 2)]/*, bitangents[indices.at(i + 2)]*/, numTimesUsed[indices.at(i + 2)]);
		/*vec3 edge1 = vertex[indices.at(i + 1)] - vertex[indices.at(i)];
		vec3 edge2 = vertex[indices.at(i + 2)] - vertex[indices.at(i)];

		vec2 deltaUV1 = texture[indices.at(i + 1)] - texture[indices.at(i)];
		vec2 deltaUV2 = texture[indices.at(i + 2)] - texture[indices.at(i)];

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		
		vec3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = normalize(tangent);*/
	}
}

//Calculate tangent
void ModelLoader::CalculateTangent(const glm::vec3 & vertex1, const glm::vec3 & vertex2, const glm::vec3 & vertex3, const glm::vec2 & texture1, const  glm::vec2 & texture2, const glm::vec2 & texture3, glm::vec3 & tangent/*, vec3 & bitangent*/, int & numTimesUsed)
{
	const glm::vec3 edge1 = vertex2 - vertex1;
	const glm::vec3 edge2 = vertex3 - vertex1;

	const glm::vec2 deltaUV1 = texture2 - texture1;
	const glm::vec2 deltaUV2 = texture3 - texture1;

	const float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	glm::vec3 newTangent;
	newTangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	newTangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	newTangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	newTangent = normalize(newTangent);

	/*vec3 newBitangent;
	newBitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	newBitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	newBitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	newBitangent = normalize(newBitangent);*/

	tangent *= numTimesUsed;
	tangent = normalize(tangent + newTangent);

	/*bitangent *= numTimesUsed;
	bitangent = normalize(bitangent + newBitangent);*/
	numTimesUsed++;
}

//Turns GLM Mat4 to Assimp Mat4
aiMatrix4x4 ModelLoader::GLMMat4ToAi(const glm::mat4 & mat)
{
	return aiMatrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
		mat[1][0], mat[1][1], mat[1][2], mat[1][3],
		mat[2][0], mat[2][1], mat[2][2], mat[2][3],
		mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

//Turns Assimp Mat4 to GLM Mat4
glm::mat4 ModelLoader::AiToGLMMat4(const aiMatrix4x4 & mat)
{
	glm::mat4 tmp;
	tmp[0][0] = mat.a1;
	tmp[1][0] = mat.b1;
	tmp[2][0] = mat.c1;
	tmp[3][0] = mat.d1;

	tmp[0][1] = mat.a2;
	tmp[1][1] = mat.b2;
	tmp[2][1] = mat.c2;
	tmp[3][1] = mat.d2;

	tmp[0][2] = mat.a3;
	tmp[1][2] = mat.b3;
	tmp[2][2] = mat.c3;
	tmp[3][2] = mat.d3;

	tmp[0][3] = mat.a4;
	tmp[1][3] = mat.b4;
	tmp[2][3] = mat.c4;
	tmp[3][3] = mat.d4;

	return tmp;
}

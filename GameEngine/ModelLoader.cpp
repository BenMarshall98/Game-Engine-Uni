#include "ModelLoader.h"


#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <fstream>
#include <sstream>

using namespace Assimp;

iModel * ModelLoader::LoadModel(const string & fileName)
{

	//TODO Add more file types (COLLADA and custom animated model file format)
	if (EndsWith(fileName, ".obj"))
	{
		return LoadOBJ(fileName);
	}
	else if (EndsWith(fileName, ".sme"))
	{
		return LoadSME(fileName);
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

StaticModel * ModelLoader::LoadOBJ(const string & fileName)
{
	ifstream reader(fileName.c_str());

	if (reader.fail())
	{
		//TODO: Log that the file does not exist
		reader.close();
		return nullptr;
	}

	vector<string> vertexString;
	vector<string> textureString;
	vector<string> normalString;
	vector<string> faceString;

	while (!reader.eof())
	{
		string line;
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

	vector<string> modelData;
	vector<int> indexes;

	for (int i = 0; i < faceString.size(); i++)
	{
		int vertex;
		int texture;
		int normal;
		char slash;

		istringstream line(faceString[i]);
		for (int j = 0; j < 3; j++)
		{
			line >> vertex >> slash >> texture >> slash >> normal;

			if (normal == 0)
			{
				//TODO: Log that the .obj file needs to include both normals and texture coordinates
				return nullptr;
			}

			string lineToAdd = vertexString[vertex - 1] + " " + textureString[texture - 1] + " " + normalString[normal - 1];
			int index = FindInVector(modelData, lineToAdd);
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

	vector<vec3> vertex;
	vector<vec2> texture;
	vector<vec3> normal;

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

		istringstream line(modelData[i]);
		line >> vertexX >> vertexY >> vertexZ >> textureX >> textureY >> normalX >> normalY >> normalZ;

		vertex.push_back(vec3(vertexX, vertexY, vertexZ));
		texture.push_back(vec2(textureX, textureY));
		normal.push_back(vec3(normalX, normalY, normalZ));
	}

	vector<vec3> tangents;
	/*vector<vec3> bitangents;*/

	TangentSpace(indexes, vertex, texture, tangents/*, bitangents*/);

	return new StaticModel(vertex, texture, normal, indexes, tangents/*, bitangents*/);
}

int ModelLoader::FindInVector(vector<string> & list, const string & toFind)
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

StaticModel * ModelLoader::LoadSME(const string & fileName)
{
	//fileName = ""; //TODO: Remove this line, this was only placed to remove a parasoft issue
	return nullptr;
}

AnimatedModel * ModelLoader::LoadDAE(const string & fileName)
{
	Importer import;
	const aiScene * scene = import.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return nullptr;
	}

	AnimatedModel * animatedModel = new AnimatedModel();

	vector<aiNode*> nodes;

	vector<Bone *> bones;

	RecursiveNodeProcess(nodes, scene->mRootNode);
	AnimNodeProcess(animatedModel, scene);

	mat4 globalInverseTransform = AnimatedModel::AiToGLMMat4(scene->mRootNode->mTransformation);

	ProcessNode(scene->mRootNode, scene, animatedModel);

	Animation firstAnimation = animatedModel->GetFirstAnimation();

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++)
		{
			string boneName = scene->mMeshes[i]->mBones[j]->mName.data;

			mat4 boneMat = AnimatedModel::AiToGLMMat4(scene->mMeshes[i]->mBones[j]->mOffsetMatrix);

			Bone * bone = new Bone();
			bone->name = boneName;
			bone->offset_matrix = boneMat;
			bone->mesh = animatedModel->GetMesh(i);
			bone->node = FindAINode(nodes, boneName);
			bone->animNode = FindAiNodeAnim(firstAnimation.animNodes, boneName);

			bones.push_back(bone);
		}
	}

	for (int i = 0; i < bones.size(); i++)
	{
		string boneName = bones.at(i)->name;
		string parentName = FindAINode(nodes, boneName)->mParent->mName.data;

		Bone * parentBone = FindBone(bones, parentName);

		bones.at(i)->parent_bone = parentBone;
	}

	animatedModel->SetBones(bones);
	animatedModel->SetNodes(nodes);
	animatedModel->SetGlobalInverse(globalInverseTransform);

	animatedModel->LoadModel();
	
	return animatedModel;
}

void ModelLoader::ProcessNode(aiNode * node, const aiScene * scene, AnimatedModel * animatedModel)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		animatedModel->AddMesh(ProcessMesh(node, mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, animatedModel);
	}
}

Mesh ModelLoader::ProcessMesh(aiNode * node, aiMesh * pMesh, const aiScene * scene)
{
	Mesh mesh;

	mesh.globalInverse = inverse(AnimatedModel::AiToGLMMat4(node->mParent->mTransformation));

	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		aiVector3D aiVertex = pMesh->mVertices[i];
		aiVector3D aiNormal = pMesh->mNormals[i];
		aiVector3D * aiTexture = pMesh->mTextureCoords[i];

		vec3 vertex = vec3(aiVertex.x, aiVertex.y, aiVertex.z);
		vec3 normal = vec3(aiVertex.x, aiVertex.y, aiVertex.z);
		vec3 texture = vec3(aiVertex.x, aiVertex.y, aiVertex.z);

		mesh.vertex.push_back(vertex);
		mesh.normal.push_back(normal);
		mesh.textures.push_back(texture);
	}

	for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
	{
		aiFace face = pMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			mesh.indices.push_back(face.mIndices[j]);
		}
	}

	const int WEIGHTS_PER_VERTEX = 4;
	int boneArraysSize = pMesh->mNumVertices * WEIGHTS_PER_VERTEX;

	vector<int> boneIDs;
	boneIDs.resize(boneArraysSize);

	vector<float>boneWeights;
	boneWeights.resize(boneArraysSize);

	vector<ivec4> IDs;
	IDs.resize(pMesh->mNumVertices);

	vector<vec4> Weights;
	Weights.resize(pMesh->mNumVertices);

	for (int i = 0; i < pMesh->mNumBones; i++)
	{
		aiBone * aiBone = pMesh->mBones[i];

		for (int j = 0; j < aiBone->mNumWeights; j++)
		{
			aiVertexWeight weight = aiBone->mWeights[j];

			unsigned int vertexStart = weight.mVertexId * WEIGHTS_PER_VERTEX;

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

	mesh.ids = IDs;
	mesh.weights = Weights;

	return mesh;
}


void ModelLoader::RecursiveNodeProcess(vector<aiNode*> & nodes, aiNode * node)
{
	aiNode * mNode = new aiNode(*node);
	nodes.push_back(mNode);

	for (int i = 0; i < node->mNumChildren; i++)
	{
		RecursiveNodeProcess(nodes, node->mChildren[i]);
	}
}

void ModelLoader::AnimNodeProcess(AnimatedModel * animationModel, const aiScene * scene)
{
	if (scene->mNumAnimations == 0)
	{
		return;
	}

	for (int i = 0; i < scene->mNumAnimations; i++)
	{
		Animation animation = Animation();
		animation.name = scene->mAnimations[i]->mName.data;

		for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
		{
			animation.animNodes.push_back(scene->mAnimations[i]->mChannels[j]);
		}

		animationModel->AddAnimation(animation);
	}
}

void ModelLoader::TangentSpace(vector<int> & indices, vector<vec3> & vertex, vector<vec2> & texture, vector<vec3> & tangents/*, vector<vec3> & bitangents*/)
{
	vector<int> numTimesUsed;

	for (int i = 0; i < vertex.size(); i++)
	{
		numTimesUsed.push_back(0);
		tangents.push_back(vec3(0));
		/*bitangents.push_back(vec3(0));*/
	}

	for (int i = 0; i < indices.size(); i = i + 3)
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

void ModelLoader::CalculateTangent(vec3 & vertex1, vec3 & vertex2, vec3 & vertex3, vec2 & texture1, vec2 & texture2, vec2 & texture3, vec3 & tangent/*, vec3 & bitangent*/, int & numTimesUsed)
{
	vec3 edge1 = vertex2 - vertex1;
	vec3 edge2 = vertex3 - vertex1;

	vec2 deltaUV1 = texture2 - texture1;
	vec2 deltaUV2 = texture3 - texture1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	vec3 newTangent;
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
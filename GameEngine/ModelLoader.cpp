#include "ModelLoader.h"


#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include <fstream>
#include <sstream>

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
	AnimatedModel * model = new AnimatedModel();

	Importer importer;
	const aiScene * scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (scene)
	{
		recursiveNodeProcess(scene->mRootNode, model);

		for (int i = 0 ; i < scene->mNumAnimations; i++)
		{
			Animation * animation = new Animation();
			animation->framesPerSecond = scene->mAnimations[i]->mTicksPerSecond;
			animation->frameDuration = scene->mAnimations[i]->mDuration;

			double secondsPerFrame = 1 / animation->framesPerSecond;

			animation->timeDuration = secondsPerFrame * animation->frameDuration;
			for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
			{
				animation->nodesAnim.push_back(scene->mAnimations[i]->mChannels[j]);
			}

			model->AddAnimation(animation);
		}

		mat4 globalInverse = inverse(AnimatedModel::AiMatrixtoGLMMatrix(scene->mRootNode->mTransformation));
		model->SetGlobalInverse(globalInverse);

		const aiMesh* mesh = scene->mMeshes[0];

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D vertex = mesh->mVertices[i];
			aiVector3D texCoord(0.5f, 0.5f, 0);
			aiVector3D normal(0, 0, 0);

			if (mesh->HasTextureCoords(0))
			{
				texCoord = mesh->mTextureCoords[0][i];
			}

			if (mesh->HasNormals())
			{
				normal = mesh->mNormals[i];
			}

			vec3 vert = vec3(vertex.x, vertex.y, vertex.z);
			vec2 text = vec2(texCoord.x, texCoord.y);
			vec3 norm = vec3(normal.x, normal.y, normal.z);

			model->AddVertex(vert);
			model->AddTexture(text);
			model->AddNormal(norm);
		}

		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace& face = mesh->mFaces[i];

			model->AddIndex(face.mIndices[0]);
			model->AddIndex(face.mIndices[1]);
			model->AddIndex(face.mIndices[2]);
		}

		model->ResizeIDs(mesh->mNumVertices * 4);
		model->ResizeWeights(mesh->mNumVertices * 4);

		for (int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone * bone = mesh->mBones[i];

			for (int j = 0; j < bone->mNumWeights; j++)
			{
				aiVertexWeight weight = bone->mWeights[j];
				unsigned int vertexStart = weight.mVertexId * 4;

				for (int k = 0; k < 4; k++)
				{
					if (model->GetWeightAt(vertexStart + k) == 0)
					{
						model->SetWeightAt(vertexStart + k, weight.mWeight);
						model->SetIDAt(vertexStart + k, i);
						break;
					}
				}

			}

			Bone * newBone = new Bone;
			
			newBone->name = bone->mName.data;
			newBone->offsetMatrix = transpose(AnimatedModel::AiMatrixtoGLMMatrix(bone->mOffsetMatrix));
			newBone->node = FindNode(newBone->name, model);

			for (int k = 0; k < model->GetAnimationCount(); k++)
			{
				newBone->animNode.push_back(FindAnimNode(newBone->name, model->GetAnimation(k)));
			}

			model->AddBone(newBone);

		}

		for (int i = 0; i < model->GetBoneCount(); i++)
		{
			string name = model->GetBone(i)->name;
			string parentName = FindNode(name, model)->mParent->mName.data;

			Bone * parentBone = FindBone(parentName, model);

			model->GetBone(i)->parent = parentBone;
		}

		model->Load();
		return model;
	}
	else
	{
		//TODO: Log error occured
		return nullptr;
	}
}

void ModelLoader::recursiveNodeProcess(aiNode* node, AnimatedModel * model)
{
	model->AddNode(new aiNode(*node));

	for (int i = 0; i < node->mNumChildren; i++)
	{
		recursiveNodeProcess(node->mChildren[i], model);
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
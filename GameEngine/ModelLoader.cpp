#include "ModelLoader.h"
#include "glm/glm.hpp"

#include <fstream>
#include <sstream>

using namespace std;
using namespace glm;

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

	return new StaticModel(vertex, texture, normal, indexes);
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
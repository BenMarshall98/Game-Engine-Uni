#pragma once
#include "iModel.h"
#include "StaticModel.h"
#include "AnimatedModel.h"
#include <vector>
#include <string>
#include "glm/glm.hpp"

#include "assimp/scene.h"

using namespace std;
using namespace glm;
using namespace Assimp;

class ModelLoader
{
private:

	//The following code is adapted from:
	//https://thispointer.com/c-how-to-check-if-a-string-ends-with-an-another-given-string/
	inline static bool EndsWith(const string & fileName, const string & fileType)
	{
		return (fileName.size() >= fileType.size() &&
			fileName.compare(fileName.size() - fileType.size(), fileType.size(), fileType) == 0);
	}

	//static void SimpleFormatExists(const string & fileName, const string & fileType);
	static int FindInVector(vector<string> & list, const string & toFind);
	static StaticModel * LoadOBJ(const string & fileName);
	static StaticModel * LoadSME(const string & fileName);
	static AnimatedModel * LoadDAE(const string & fileName);

	static void TangentSpace(vector<int> & indices, vector<vec3> & vertex, vector<vec2> & texture, vector<vec3> & tangents/*, vector<vec3> & bitangents*/);
	static void CalculateTangent(vec3 & vertex1, vec3 & vertex2, vec3 & vertex3, vec2 & texture1, vec2 & texture2, vec2 & texture3, vec3 & tangent/*, vec3 & bitangent*/, int & numTimesUsed);

	static void ProcessNode(aiNode * node, const aiScene * scene, AnimatedModel * animatedModel);
	static Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene);
public:
	static iModel * LoadModel(const string & fileName);
};


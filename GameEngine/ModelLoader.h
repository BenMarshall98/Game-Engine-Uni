#pragma once
#include "iModel.h"
#include "StaticModel.h"
#include "AnimatedModel.h"
#include "assimp/scene.h"
#include <vector>
#include <string>
#include "glm/glm.hpp"

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

	static mat4 AiMatrixtoGLMMatrix(aiMatrix4x4& matrix)
	{
		mat4 newMatrix;
		newMatrix[0][0] = matrix.a1;
		newMatrix[1][0] = matrix.b1;
		newMatrix[2][0] = matrix.c1;
		newMatrix[3][0] = matrix.d1;

		newMatrix[0][1] = matrix.a2;
		newMatrix[1][1] = matrix.b2;
		newMatrix[2][1] = matrix.c2;
		newMatrix[3][1] = matrix.d2;

		newMatrix[0][2] = matrix.a3;
		newMatrix[1][2] = matrix.b3;
		newMatrix[2][2] = matrix.c3;
		newMatrix[3][2] = matrix.d3;

		newMatrix[0][3] = matrix.a4;
		newMatrix[1][3] = matrix.b4;
		newMatrix[2][3] = matrix.c4;
		newMatrix[3][3] = matrix.d4;

		return newMatrix;
	}

	static void recursiveNodeProcess(aiNode* node, AnimatedModel * model);

	static Bone* FindBone(string name, AnimatedModel * model)
	{
		for (int i = 0; i < model->bones.size(); i++)
		{
			if (model->bones.at(i)->name == name)
			{
				return model->bones.at(i);
			}
			return nullptr;
		}
	}

	static aiNode* FindNode(string name, AnimatedModel * model)
	{
		for (int i = 0; i < model->nodes.size(); i++)
		{
			if (model->nodes.at(i)->mName.data == name)
			{
				return model->nodes.at(i);
			}
		}
		return nullptr;
	}

	static aiNodeAnim *FindAnimNode(string name, AnimatedModel * model)
	{
		for (int i = 0; i < model->nodesAnim.size(); i++)
		{
			if (model->nodesAnim.at(i)->mNodeName.data == name)
			{
				return model->nodesAnim.at(i);
			}
		}
		return nullptr;
	}

	static int FindBoneIDByName(string name, AnimatedModel * model)
	{
		for (int i = 0; i < model->bones.size(); i++)
		{
			if (model->bones.at(i)->name == name)
			{
				return i;
			}
		}
		return -1;
	}

	ModelLoader(void) {};
public:
	static iModel * LoadModel(const string & fileName);
};


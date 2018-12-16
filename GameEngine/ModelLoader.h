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

	static void TangentSpace(vector<int> & indices, vector<vec3> & vertex, vector<vec2> & texture, vector<vec3> & tangents);

	static void recursiveNodeProcess(aiNode* node, AnimatedModel * model);

	static Bone* FindBone(string & name, AnimatedModel * model)
	{
		for (int i = 0; i < model->bones.size(); i++)
		{
			if (model->bones.at(i)->name == name)
			{
				return model->bones.at(i);
			}
		}
		return nullptr;
	}

	static aiNode* FindNode(string & name, AnimatedModel * model)
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

	static aiNodeAnim *FindAnimNode(string & name, Animation * animation)
	{
		for (int i = 0; i < animation->nodesAnim.size(); i++)
		{
			if (animation->nodesAnim.at(i)->mNodeName.data == name)
			{
				return animation->nodesAnim.at(i);
			}
		}
		return nullptr;
	}

	static int FindBoneIDByName(string & name, AnimatedModel * model)
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

public:
	static iModel * LoadModel(const string & fileName);
};


#pragma once
#include "iModel.h"
#include "StaticModel.h"
#include "AnimatedModel.h"
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "assimp/scene.h"

class ModelLoader
{
private:

	//The following code is adapted from:
	//https://thispointer.com/c-how-to-check-if-a-string-ends-with-an-another-given-string/
	inline static bool EndsWith(const std::string & fileName, const std::string & fileType)
	{
		return (fileName.size() >= fileType.size() &&
			fileName.compare(fileName.size() - fileType.size(), fileType.size(), fileType) == 0);
	}

	//static void SimpleFormatExists(const string & fileName, const string & fileType);
	static int FindInVector(std::vector<std::string> & list, const std::string & toFind);
	static StaticModel * LoadOBJ(const std::string & fileName);
	static StaticModel * LoadSME(const std::string & fileName);
	static AnimatedModel * LoadDAE(const std::string & fileName);

	static void TangentSpace(std::vector<int> & indices, std::vector<glm::vec3> & vertex, std::vector<glm::vec2> & texture, std::vector<glm::vec3> & tangents/*, vector<vec3> & bitangents*/);
	static void CalculateTangent(const glm::vec3 & vertex1, const glm::vec3 & vertex2, const glm::vec3 & vertex3, const glm::vec2 & texture1, const glm::vec2 & texture2, const glm::vec2 & texture3, glm::vec3 & tangent/*, vec3 & bitangent*/, int & numTimesUsed);

	static void ProcessNode(aiNode * node, const aiScene * scene, AnimatedModel * animatedModel);
	static Mesh * ProcessMesh(aiNode * node, aiMesh * mesh, const aiScene * scene);

	static aiMatrix4x4 GLMMat4ToAi(glm::mat4 mat);
	static glm::mat4 AiToGLMMat4(aiMatrix4x4& mat);

	static void RecursiveNodeProcess(std::vector<Node*> & nodes, aiNode * node, Node * parentNode);
	static void AnimNodeProcess(AnimatedModel * animationModel, const aiScene * scene);

	static inline Bone * FindBone(std::vector<Bone *> bones, const std::string name)
	{
		for (int i = 0; i < bones.size(); i++)
		{
			if (bones.at(i)->GetName() == name)
			{
				return bones.at(i);
			}
		}

		return nullptr;
	}

	static inline Node* FindNode(std::vector<Node*> nodes, const std::string name)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			if (nodes.at(i)->GetName() == name)
			{
				return nodes.at(i);
			}
		}

		return nullptr;
	}

	static inline AnimNode* FindAiNodeAnim(std::vector<AnimNode*> animNodes, const std::string name)
	{
		for (int i = 0; i < animNodes.size(); i++)
		{
			if (animNodes.at(i)->GetName() == name)
			{
				return animNodes.at(i);
			}
		}

		return nullptr;
	}

	static inline int FindBoneIDByName(std::vector<Bone *> bones, const std::string name)
	{
		for (int i = 0; i < bones.size(); i++)
		{
			if (bones.at(i)->GetName() == name)
			{
				return i;
			}
		}

		return -1;
	}

public:
	static iModel * LoadModel(const std::string & fileName);
};


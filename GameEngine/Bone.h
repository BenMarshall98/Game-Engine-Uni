#pragma once

#include "AnimNode.h"
#include "Node.h"
#include "glm/glm.hpp"
#include <string>

class Bone
{
private:
	std::string name;
	Node * node;
	AnimNode * animNode;
	Bone * parentBone;
	glm::mat4 offsetMatrix;

public:
	Bone(std::string pName, Node * const pNode, AnimNode * const pAnimNode, glm::mat4 pOffsetMatrix) :
		name(pName), node(pNode), animNode(pAnimNode), offsetMatrix(pOffsetMatrix), parentBone(nullptr)
	{

	}

	~Bone();

	inline void SetParentBone(Bone * const pParentBone)
	{
		parentBone = pParentBone;
	}

	inline std::string GetName()
	{
		return name;
	}

	inline glm::mat4 GetOffsetMatrix()
	{
		return offsetMatrix;
	}

	inline Node * GetNode()
	{
		return node;
	}

	glm::mat4 GetParentTransforms();

	unsigned int FindPosition(float time);
	glm::vec3 CalcInterpolatedPosition(float time);
	unsigned int FindRotation(float time);
	glm::quat CalcInterpolatedRotation(float time);

	void UpdateKeyframeTransform(float time);
};
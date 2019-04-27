#pragma once

#include "AnimNode.h"
#include "Node.h"
#include "glm/glm.hpp"
#include "Animation.h"
#include <string>
#include <map>

class Bone
{
private:
	std::string name;
	Node * node;
	std::map<Animation *, AnimNode *> animNodes;
	Bone * parentBone;
	glm::mat4 offsetMatrix;

public:
	Bone(std::string & pName, Node * const pNode, const std::map<Animation *, AnimNode *> & pAnimNodes, glm::mat4 & pOffsetMatrix) :
		name(pName), node(pNode), animNodes(pAnimNodes), offsetMatrix(pOffsetMatrix), parentBone(nullptr)
	{

	}

	~Bone();

	Bone& operator=(const Bone&) = delete;
	Bone(Bone&) = delete;

	//Sets the parent bone
	inline void SetParentBone(Bone * const pParentBone)
	{
		parentBone = pParentBone;
	}

	//Gets the bone name
	inline std::string & GetName()
	{
		return name;
	}

	//Gets the offset matrix
	inline glm::mat4 & GetOffsetMatrix()
	{
		return offsetMatrix;
	}

	//Gets the related node
	inline Node * GetNode()
	{
		return node;
	}

	glm::mat4 GetParentTransforms();

	unsigned int FindPosition(AnimNode * animNode, float time);
	glm::vec3 CalcInterpolatedPosition(AnimNode * animNode, float time);
	unsigned int FindRotation(AnimNode * animNode, float time);
	glm::quat CalcInterpolatedRotation(AnimNode * animNode, float time);
	unsigned int FindScale(AnimNode * animNode, float time);
	glm::vec3 CalcInterpolatedScale(AnimNode * animNode, float time);

	void UpdateKeyframeTransform(Animation * animation, float time);
};
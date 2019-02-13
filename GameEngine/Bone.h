#pragma once

#include "assimp/scene.h"
#include "AnimNode.h"
#include "glm/glm.hpp"
#include <string>

using namespace glm;
using namespace Assimp;

class Bone
{
private:
	string name;
	aiNode * node;
	AnimNode * animNode;
	Bone * parentBone;
	mat4 offsetMatrix;

public:
	Bone(string pName, aiNode * pNode, AnimNode * pAnimNode, mat4 pOffsetMatrix) :
		name(pName), node(pNode), animNode(pAnimNode), offsetMatrix(pOffsetMatrix), parentBone(nullptr)
	{

	}

	~Bone();

	inline void SetParentBone(Bone * pParentBone)
	{
		parentBone = pParentBone;
	}

	inline string GetName()
	{
		return name;
	}

	inline mat4 GetOffsetMatrix()
	{
		return offsetMatrix;
	}

	inline aiNode * GetNode()
	{
		return node;
	}

	mat4 GetParentTransforms();

	unsigned int FindPosition(float time);
	vec3 CalcInterpolatedPosition(float time);
	unsigned int FindRotation(float time);
	quat CalcInterpolatedRotation(float time);

	void UpdateKeyframeTransform(float time);
};
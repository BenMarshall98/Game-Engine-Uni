#pragma once

#include "Vec3AnimKey.h"
#include "QuatAnimKey.h"
#include <vector>
#include <string>

class AnimNode
{
private:
	std::string name;
	std::vector<Vec3AnimKey *> PositionKeys;
	std::vector<QuatAnimKey *> RotationKeys;
	std::vector<Vec3AnimKey *> ScaleKeys;

public:
	AnimNode(std::string & pName, std::vector<Vec3AnimKey *> & pPositionKeys, std::vector<QuatAnimKey *> & pRotationKeys, std::vector<Vec3AnimKey *> & pScaleKeys) :
		name(pName), PositionKeys(pPositionKeys), RotationKeys(pRotationKeys), ScaleKeys(pScaleKeys) {}
	~AnimNode();

	AnimNode& operator=(const AnimNode&) = delete;
	AnimNode(AnimNode&) = delete;

	//Gets animnode name
	inline std::string & GetName()
	{
		return name;
	}

	//Gets position keys
	inline std::vector<Vec3AnimKey *> & GetPositionKeys()
	{
		return PositionKeys;
	}

	//Gets rotation keys
	inline std::vector<QuatAnimKey *> & GetRotationKeys()
	{
		return RotationKeys;
	}

	//Gets scale keys
	inline std::vector<Vec3AnimKey *> & GetScaleKeys()
	{
		return ScaleKeys;
	}
};
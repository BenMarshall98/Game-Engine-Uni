#pragma once

#include "Vec3AnimKey.h"
#include "QuatAnimKey.h"
#include <vector>
#include <string>

using namespace std;

class AnimNode
{
private:
	string name;
	vector<Vec3AnimKey *> PositionKeys;
	vector<QuatAnimKey *> RotationKeys;
	vector<Vec3AnimKey *> ScaleKeys;

public:
	AnimNode(string pName, vector<Vec3AnimKey *> pPositionKeys, vector<QuatAnimKey *> pRotationKeys, vector<Vec3AnimKey *> pScaleKeys) :
		name(pName), PositionKeys(pPositionKeys), RotationKeys(pRotationKeys), ScaleKeys(pScaleKeys) {}
	~AnimNode();

	inline string GetName()
	{
		return name;
	}

	inline vector<Vec3AnimKey *> GetPositionKeys()
	{
		return PositionKeys;
	}

	inline vector<QuatAnimKey *> GetRotationKeys()
	{
		return RotationKeys;
	}

	inline vector<Vec3AnimKey *> GetScaleKeys()
	{
		return ScaleKeys;
	}
};
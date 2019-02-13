#pragma once

#include <vector>
#include <string>

#include "AnimNode.h"

using namespace std;

class Animation
{
private:
	string name;
	float startTime;
	float endTime;
	vector<AnimNode *> animNodes;

public:
	Animation(string pName, float pStartTime, float pEndTime, vector<AnimNode *> pAnimNodes) :
		name(pName), startTime(pStartTime), endTime(pEndTime), animNodes(pAnimNodes)
	{

	}

	~Animation();

	inline string GetName()
	{
		return name;
	}

	inline float GetStartTime()
	{
		return startTime;
	}

	inline float GetEndTime()
	{
		return endTime;
	}

	inline vector<AnimNode *> GetAnimNodes()
	{
		return animNodes;
	}
};

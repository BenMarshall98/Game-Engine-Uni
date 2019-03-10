#pragma once

#include <vector>
#include <string>

#include "AnimNode.h"

class Animation
{
private:
	std::string name;
	float startTime;
	float endTime;
	std::vector<AnimNode *> animNodes;

public:
	Animation(std::string pName, const float pStartTime, const float pEndTime, std::vector<AnimNode *> pAnimNodes) :
		name(pName), startTime(pStartTime), endTime(pEndTime), animNodes(pAnimNodes)
	{

	}

	~Animation();

	inline std::string GetName()
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

	inline std::vector<AnimNode *> GetAnimNodes()
	{
		return animNodes;
	}
};

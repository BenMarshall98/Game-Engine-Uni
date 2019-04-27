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
	Animation(std::string & pName, const float pStartTime, const float pEndTime, std::vector<AnimNode *> & pAnimNodes) :
		name(pName), startTime(pStartTime), endTime(pEndTime), animNodes(pAnimNodes)
	{

	}

	~Animation();

	Animation& operator=(const Animation&) = delete;
	Animation(Animation&) = delete;

	//Gets animation name
	inline std::string & GetName()
	{
		return name;
	}

	//Gets animation start time
	inline float GetStartTime()
	{
		return startTime;
	}

	//Gets animation end time
	inline float GetEndTime()
	{
		return endTime;
	}

	//Gets animation nodes
	inline std::vector<AnimNode *> & GetAnimNodes()
	{
		return animNodes;
	}
};

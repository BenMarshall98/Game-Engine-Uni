#pragma once

#include <string>
#include "glm/glm.hpp"
#include "ComponentPhysics.h"
#include "PathFinding.h"
#include "PathFollowing.h"
#include <string>
#include <map>

using namespace std;
using namespace glm;

class AIStateMachine
{
private:
	map<string, string> stateValues = map<string, string>();
	string startFunction;
	PathFinding * pathFinder;
	PathFollowing * pathFollower;

public:
	explicit AIStateMachine(string pStartFunction) : startFunction(pStartFunction) {}
	~AIStateMachine();

	//void TraverseStates(vec3 currentPosition, quat currentDirection, ComponentPhysics * physicsComponent);
	string GetValue(string valueName, string defaultValue);
	void SetValue(string valueName, string value);

	inline void SetPathFinder(PathFinding * pPathFinder)
	{
		pathFinder = pPathFinder;
	}

	inline void SetPathFollower(PathFollowing * pPathFollower)
	{
		pathFollower = pPathFollower;
	}

	void MoveOffPath();

	inline bool OnPath(vec3 position)
	{
		if (pathFollower)
		{
			return pathFollower->OnPath(position);
		}
		return false;
	}
};


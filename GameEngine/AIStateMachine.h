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
	explicit AIStateMachine(string pStartFunction) : startFunction(pStartFunction), pathFinder(nullptr), pathFollower(nullptr) {}
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

	void FindAIPath(vec3 position, quat direction, ComponentPhysics * physicsComponent, vec3 target, float deltaTime);
	void FindAIPath(vec3 position, quat direction, ComponentPhysics * physicsComponent, float deltaTime);
	void FindPath(vec3 position, quat direction, ComponentPhysics * physicsComponent, float deltaTime);

	vec3 GetNearestPath(vec3 position);

	void ResolveStates(Entity * thisEntity, Entity * target);
};


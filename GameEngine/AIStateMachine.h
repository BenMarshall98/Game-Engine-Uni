#pragma once

#include <string>
#include "glm/glm.hpp"
#include "ComponentPhysics.h"
#include "PathFinding.h"
#include "PathFollowing.h"
#include <string>
#include <map>

class AIStateMachine
{
private:
	std::map<std::string, std::string> stateValues = std::map<std::string, std::string>();
	std::string startFunction;
	PathFinding * pathFinder;
	PathFollowing * pathFollower;

public:
	explicit AIStateMachine(std::string pStartFunction) : startFunction(pStartFunction), pathFinder(nullptr), pathFollower(nullptr) {}
	~AIStateMachine();

	//void TraverseStates(vec3 currentPosition, quat currentDirection, ComponentPhysics * physicsComponent);
	std::string GetValue(std::string valueName, std::string defaultValue);
	void SetValue(std::string valueName, std::string value);

	inline void SetPathFinder(PathFinding * pPathFinder)
	{
		pathFinder = pPathFinder;
	}

	inline void SetPathFollower(PathFollowing * pPathFollower)
	{
		pathFollower = pPathFollower;
	}

	void MoveOffPath();

	inline bool OnPath(glm::vec3 position)
	{
		if (pathFollower)
		{
			return pathFollower->OnPath(position);
		}
		return false;
	}

	void FindAIPath(glm::vec3 position, glm::quat direction, ComponentPhysics * physicsComponent, glm::vec3 target, float deltaTime);
	void FindAIPath(glm::vec3 position, glm::quat direction, ComponentPhysics * physicsComponent, float deltaTime);
	void FindPath(glm::vec3 position, glm::quat direction, ComponentPhysics * physicsComponent, float deltaTime);

	glm::vec3 GetNearestPath(glm::vec3 position);

	void ResolveStates(Entity * thisEntity, Entity * target);
};


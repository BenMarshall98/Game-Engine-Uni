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
	explicit AIStateMachine(std::string & pStartFunction) : startFunction(pStartFunction), pathFinder(nullptr), pathFollower(nullptr) {}
	~AIStateMachine();

	AIStateMachine& operator=(const AIStateMachine&) = delete;
	AIStateMachine(AIStateMachine&) = delete;

	std::string GetValue(std::string & valueName, std::string & defaultValue);
	void SetValue(std::string & valueName, std::string & value);

	//Sets the path finder to use
	inline void SetPathFinder(PathFinding * const pPathFinder)
	{
		pathFinder = pPathFinder;
	}

	//Sets the path follower to use
	inline void SetPathFollower(PathFollowing * const pPathFollower)
	{
		pathFollower = pPathFollower;
	}

	void MoveOffPath();

	//Checks if entity is on its path
	inline bool OnPath(const glm::vec3 & position)
	{
		if (pathFollower)
		{
			return pathFollower->OnPath(position);
		}
		return false;
	}

	void FindAIPath(const glm::vec3 & position, const glm::quat & direction, ComponentPhysics * const physicsComponent, const glm::vec3 & target, float deltaTime);
	void FindAIPath(const glm::vec3 & position, const glm::quat & direction, ComponentPhysics * const physicsComponent, float deltaTime);
	void FindPath(const glm::vec3 & position, const glm::quat & direction, ComponentPhysics * const physicsComponent, float deltaTime);

	glm::vec3 GetNearestPath(const glm::vec3 & position);

	void ResolveStates(Entity * thisEntity, Entity * target, float timePeriod);
};


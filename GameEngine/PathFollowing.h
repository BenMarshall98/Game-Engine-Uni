#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "ComponentPhysics.h"

struct PathNode
{
	PathNode() : radius(0) {}
	glm::vec3 position;
	float radius;
};

class PathFollowing
{
private:
	std::vector<PathNode *> * pathNodes;
	int currentNode;
	bool onPath;

public:
	explicit PathFollowing(std::vector<PathNode *> * const pPathNodes) : pathNodes(pPathNodes), currentNode(-1), onPath(true) {}
	~PathFollowing();

	PathFollowing& operator=(const PathFollowing&) = delete;
	PathFollowing(PathFollowing&) = delete;

	void CalculatePath(const glm::vec3 & currentPosition, const glm::quat & currentDirection, ComponentPhysics * const physicsComponent);

	//Moves entity off of path
	inline void MoveOffPath()
	{
		onPath = false;
	}

	bool OnPath(const glm::vec3 & position);

	glm::vec3 GetNearestPath(const glm::vec3 & position);
};


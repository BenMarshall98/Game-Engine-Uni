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

	void CalculatePath(const glm::vec3 & currentPosition, const glm::quat & currentDirection, ComponentPhysics * const physicsComponent);

	inline void MoveOffPath()
	{
		onPath = false;
	}

	bool OnPath(glm::vec3 position);

	glm::vec3 GetNearestPath(glm::vec3 position);
};


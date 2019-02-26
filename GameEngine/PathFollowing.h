#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "ComponentPhysics.h"

using namespace glm;
using namespace std;

struct PathNode
{
	PathNode() : radius(0) {}
	vec3 position;
	float radius;
};

class PathFollowing
{
private:
	vector<PathNode *> pathNodes;
	int currentNode;
	bool onPath;

public:
	explicit PathFollowing(vector<PathNode *> & pPathNodes) : pathNodes(pPathNodes), currentNode(-1), onPath(true) {}
	~PathFollowing();

	void CalculatePath(const vec3 & currentPosition, const quat & currentDirection, ComponentPhysics * const physicsComponent);

	inline void MoveOffPath()
	{
		onPath = false;
	}

	bool OnPath(vec3 position);
};


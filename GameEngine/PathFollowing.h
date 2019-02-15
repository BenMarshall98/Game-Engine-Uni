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
	PathNode() {}
	vec3 position;
	float radius;
};

class PathFollowing
{
private:
	vector<PathNode *> pathNodes;
	int currentNode;

public:
	explicit PathFollowing(vector<PathNode *> pPathNodes) : pathNodes(pPathNodes), currentNode(-1) {}
	~PathFollowing();

	void CalculatePath(vec3 currentPosition, quat currentDirection, ComponentPhysics * physicsComponent);
};


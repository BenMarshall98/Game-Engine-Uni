#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "ComponentPhysics.h"
#include "Entity.h"
#include "EntityManager.h"
#include <vector>
#include <string>

using namespace std;
using namespace glm;

struct StarNode
{
	StarNode() {}
	vec3 position;
	float weight;
	StarNode * parent;
};

class PathFinding
{
private:
	vector<vector<int>> map;
	vec2 topLeftCoord;
	Entity * target;

public:
	PathFinding(string & pTarget, string & pFile, vec2 & pTopLeftCoord);
	~PathFinding();

	void CalculatePath(vec3 currentPosition, quat currentDirection, ComponentPhysics * physicsComponent);
};


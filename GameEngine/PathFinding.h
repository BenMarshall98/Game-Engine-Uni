#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "ComponentPhysics.h"
#include "Entity.h"
#include "EntityManager.h"
#include <vector>
#include <string>
#include <math.h>

using namespace std;
using namespace glm;

struct StarNode
{
	StarNode() {}
	vec3 position;
	float travelTo;
	float travelFrom;
	float weight;
	StarNode * parent;
};

class PathFinding
{
private:
	vector<vector<int>> map;
	const vector<ivec2> directions{ ivec2(1,0), ivec2(-1, 0), ivec2(0, 1), ivec2(0, -1) };
	vec2 topLeftCoord;
	Entity * target;

	ivec2 CalculateMapLoc(vec3 position);

public:
	PathFinding(string & pTarget, string & pFile, vec2 & pTopLeftCoord);
	~PathFinding();

	void CalculatePath(vec3 currentPosition, quat currentDirection, ComponentPhysics * physicsComponent);
};


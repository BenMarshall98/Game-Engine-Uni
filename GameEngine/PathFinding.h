#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "ComponentPhysics.h"
#include "Entity.h"
#include "EntityManager.h"
#include <vector>
#include <string>
#include <math.h>

struct StarNode
{
	StarNode() : parent(nullptr), travelTo(0), travelFrom(0), weight(0) {}
	glm::vec3 position;
	float travelTo;
	float travelFrom;
	float weight;
	StarNode * parent;
};

class PathFinding
{
private:
	std::vector< std::vector<int>> map;
	const std::vector<glm::ivec2> directions{ glm::ivec2(1,0), glm::ivec2(-1, 0), glm::ivec2(0, 1), glm::ivec2(0, -1) };
	glm::vec2 topLeftCoord;
	Entity * target;

	glm::ivec2 CalculateMapLoc(glm::vec3 position);

	void BuildPath(glm::vec3 cuurentPosition, glm::quat currentDirection, ComponentPhysics * componentPhysics, glm::vec3 targetLocation);

public:
	PathFinding(std::string & pTarget, std::string & pFile, glm::vec2 & pTopLeftCoord);
	~PathFinding();

	void CalculatePath(glm::vec3 currentPosition, glm::quat currentDirection, ComponentPhysics * physicsComponent);
	void CalculatePathToPosition(glm::vec3 cuurentPosition, glm::quat currentDirection, ComponentPhysics * componentPhysics, glm::vec3 targetLocation);
};


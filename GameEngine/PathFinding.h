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
	StarNode& operator=(const StarNode&) = delete;
	StarNode(StarNode&) = delete;
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

	glm::ivec2 CalculateMapLoc(const glm::vec3 & position);

	void BuildPath(const glm::vec3 & currentPosition, const glm::quat & currentDirection, ComponentPhysics * const componentPhysics, const glm::vec3 & targetLocation);

public:
	PathFinding(const std::string & pTarget, const std::string & pFile, glm::vec2 & pTopLeftCoord);
	~PathFinding();

	PathFinding& operator=(const PathFinding&) = delete;
	PathFinding(PathFinding&) = delete;

	void CalculatePath(const glm::vec3 & currentPosition, const glm::quat & currentDirection, ComponentPhysics * physicsComponent);
	void CalculatePathToPosition(const glm::vec3 & currentPosition, const glm::quat & currentDirection, ComponentPhysics * componentPhysics, const glm::vec3 & targetLocation);
};


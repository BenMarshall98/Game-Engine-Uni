#pragma once
#include "CollisionShape.h"

class CollisionCuboid : public CollisionShape
{
private:
	vec3 boundingBox;

public:
	CollisionCuboid(vec3 pBoundingBox) : CollisionShape(CUBOID), boundingBox(pBoundingBox) {}
	~CollisionCuboid();

	inline vec3 GetSize()
	{
		return boundingBox;
	}
};


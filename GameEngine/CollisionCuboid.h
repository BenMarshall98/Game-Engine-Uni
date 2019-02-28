#pragma once
#include "CollisionShape.h"
#include "glm/glm.hpp"

using namespace glm;

class CollisionCuboid : public CollisionShape
{
private:
	vec3 boundingBox;

public:
	explicit CollisionCuboid(vec3 & pBoundingBox) : boundingBox(pBoundingBox) {}
	~CollisionCuboid();

	inline const vec3 & GetSize() const
	{
		return boundingBox;
	}

	Shape GetShape() override;
};


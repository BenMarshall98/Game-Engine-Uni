#pragma once
#include "CollisionShape.h"
#include "glm/glm.hpp"

class CollisionCuboid : public CollisionShape
{
private:
	glm::vec3 boundingBox;

public:
	explicit CollisionCuboid(glm::vec3 & pBoundingBox) : boundingBox(pBoundingBox) {}
	~CollisionCuboid();

	inline const glm::vec3 & GetSize() const
	{
		return boundingBox;
	}

	Shape GetShape() override;
};


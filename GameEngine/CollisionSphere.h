#pragma once
#include "CollisionShape.h"

class CollisionSphere : public CollisionShape
{
private:
	float radius;

public:
	explicit CollisionSphere(const float pRadius) : radius(pRadius) {}
	~CollisionSphere();

	inline float GetRadius() const
	{
		return radius;
	}

	Shape GetShape() override;
};


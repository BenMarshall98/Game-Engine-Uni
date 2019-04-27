#pragma once
#include "CollisionShape.h"

class CollisionSphere : public CollisionShape
{
private:
	float radius;

public:
	explicit CollisionSphere(const float pRadius) : radius(pRadius) {}
	~CollisionSphere();

	//Gets the radius of the collision shape
	inline float GetRadius() const
	{
		return radius;
	}

	Shape GetShape() override;
};


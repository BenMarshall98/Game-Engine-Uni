#pragma once
#include "CollisionShape.h"

class CollisionSphere : public CollisionShape
{
private:
	float radius;

public:
	CollisionSphere(float pRadius) : CollisionShape(SPHERE), radius(pRadius) {}
	~CollisionSphere();

	inline float GetRadius()
	{
		return radius;
	}
};


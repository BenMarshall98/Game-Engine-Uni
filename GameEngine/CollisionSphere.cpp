#include "CollisionSphere.h"

CollisionSphere::~CollisionSphere()
{
	int i = 0;
}

//Gets the collision shape
Shape CollisionSphere::GetShape()
{
	return Shape::SPHERE;
}

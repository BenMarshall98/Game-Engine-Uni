#include "CollisionSphere.h"

CollisionSphere::~CollisionSphere()
{
	int i = 0;
}

Shape CollisionSphere::GetShape()
{
	return Shape::SPHERE;
}

#include "CollisionSphere.h"

CollisionSphere::~CollisionSphere()
{
}

Shape CollisionSphere::GetShape()
{
	return Shape::CUBOID;
}

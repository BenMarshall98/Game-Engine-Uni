#include "CollisionCuboid.h"

CollisionCuboid::~CollisionCuboid()
{
}

Shape CollisionCuboid::GetShape()
{
	return Shape::CUBOID;
}

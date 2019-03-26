#include "CollisionCuboid.h"

CollisionCuboid::~CollisionCuboid()
{
	int i = 0;
}

Shape CollisionCuboid::GetShape()
{
	return Shape::CUBOID;
}

#include "CollisionCuboid.h"

CollisionCuboid::~CollisionCuboid()
{

}

//Gets the collision shape
Shape CollisionCuboid::GetShape()
{
	return Shape::CUBOID;
}

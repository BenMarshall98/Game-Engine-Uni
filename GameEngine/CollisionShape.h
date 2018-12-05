#pragma once

enum Shape
{
	SPHERE,
	CUBOID
};

class CollisionShape
{
protected:
	Shape shape;
	CollisionShape(Shape pShape) : shape(pShape) {}

public:
	virtual ~CollisionShape();

	inline Shape GetShape()
	{
		return shape;
	}
};


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
	explicit CollisionShape(Shape pShape) : shape(pShape) {}

public:
	virtual ~CollisionShape();

	inline Shape GetShape() const
	{
		return shape;
	}
};


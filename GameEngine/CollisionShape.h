#pragma once

enum class Shape
{
	SPHERE,
	CUBOID
};

class CollisionShape
{
private:
	Shape shape;

protected:
	explicit CollisionShape(const Shape pShape) : shape(pShape) {}

public:
	virtual ~CollisionShape();

	inline Shape GetShape() const
	{
		return shape;
	}
};


#pragma once

enum class Shape
{
	SPHERE,
	CUBOID
};

class CollisionShape
{
protected:
	explicit CollisionShape() {}

public:
	virtual ~CollisionShape();

	virtual Shape GetShape() = 0;
};


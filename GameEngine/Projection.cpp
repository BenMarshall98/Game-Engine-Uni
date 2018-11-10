#include "Projection.h"

Projection::Projection(ProjectionType pProjection, float pWidth, float pHeight, float pNear, float pFar) : projection(pProjection), width(pWidth), height(pHeight), near(pNear), far(pFar)
{
	Update();
}

void Projection::Update()
{
	if (projection == ProjectionType::Orthographic)
	{
		projectionMatrix = ortho(0.0f, width, 0.0f, height, near, far);
	}
	else if (projection == ProjectionType::Perspective)
	{
		projectionMatrix = perspective(radians(45.0f), width / height, near, far);
	}
}
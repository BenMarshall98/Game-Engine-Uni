#include "Projection.h"



Projection::Projection(ProjectionType pProjection, float pWidth, float pHeight, float pNear, float pFar) : projectionType(pProjection), mWidth(pWidth), mHeight(pHeight), mNear(pNear), mFar(pFar)
{
	Update();
}

void Projection::Update()
{
	if (projectionType == ProjectionType::Orthographic)
	{
		projectionMat = ortho(0.0f, mWidth, 0.0f, mHeight, mNear, mFar);
	}
	else if (projectionType == ProjectionType::Perspective)
	{
		projectionMat = perspective(radians(45.0f), mWidth / mHeight, mNear, mFar);
	}
}

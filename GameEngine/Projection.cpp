#include "Projection.h"

Projection::Projection(const ProjectionType pProjection, const float pWidth, const float pHeight, const float pNear, const float pFar) : projectionType(pProjection), mWidth(pWidth), mHeight(pHeight), mNear(pNear), mFar(pFar)
{
	Update();
}

//Updates the projection matrix
void Projection::Update()
{
	if (mWidth == 0)
	{
		mWidth = 1;
	}
	else if (mHeight == 0)
	{
		mHeight = 1;
	}

	if (projectionType == ProjectionType::Orthographic)
	{
		projectionMat = glm::ortho(0.0f, mWidth, 0.0f, mHeight, mNear, mFar);
	}
	else if (projectionType == ProjectionType::Perspective)
	{
		projectionMat = glm::perspective(glm::radians(45.0f), mWidth / mHeight, mNear, mFar);
	}
}

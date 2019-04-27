#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class ProjectionType
{
	Orthographic,
	Perspective
};

class Projection
{
private:
	ProjectionType projectionType;
	glm::mat4 projectionMat;
	float mWidth;
	float mHeight;
	float mNear;
	float mFar;

public:
	Projection(ProjectionType pProjection, float pWidth, float pHeight, float pNear, float pFar);
	~Projection() {};

	void Update();

	//Gets the projection matrix
	inline const glm::mat4 & GetProjection() const
	{
		return projectionMat;
	}

	//Gets the projection width
	inline float GetWidth() const
	{
		return mWidth;
	}

	//Gets the projection height
	inline float GetHeight() const
	{
		return mHeight;
	}

	//Gets the projection max distance
	inline float GetFar() const
	{
		return mFar;
	}

	//Sets the projection width
	inline void SetWidth(const float pWidth)
	{
		mWidth = pWidth;
	}

	//Sets the projection height
	inline void SetHeight(const float pHeight)
	{
		mHeight = pHeight;
	}

	//Sets the projection min distance
	inline void SetNear(const float pNear)
	{
		mNear = pNear;
	}

	//Sets the projection max distance
	inline void SetFar(const float pFar)
	{
		mFar = pFar;
	}
};


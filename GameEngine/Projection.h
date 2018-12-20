#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;
using namespace std;

enum ProjectionType
{
	Orthographic,
	Perspective
};

class Projection
{
private:
	ProjectionType projectionType;
	mat4 projectionMat;
	float mWidth;
	float mHeight;
	float mNear;
	float mFar;

public:
	Projection(ProjectionType pProjection, float pWidth, float pHeight, float pNear, float pFar);
	~Projection() {};

	void Update();

	inline mat4 GetProjection()
	{
		Update(); //TODO: Remove
		return projectionMat;
	}

	inline float GetWidth() const
	{
		return mWidth;
	}

	inline float GetHeight() const
	{
		return mHeight;
	}

	inline float GetFar() const
	{
		return mFar;
	}

	inline void SetWidth(float pWidth)
	{
		mWidth = pWidth;
	}

	inline void SetHeight(float pHeight)
	{
		mHeight = pHeight;
	}

	inline void SetNear(float pNear)
	{
		mNear = pNear;
	}

	inline void SetFar(float pFar)
	{
		mFar = pFar;
	}
};


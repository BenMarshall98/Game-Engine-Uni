#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

enum ProjectionType
{
	Orthographic,
	Perspective
};

class Projection
{
private:
	ProjectionType projection;
	mat4 projectionMatrix;
	float width;
	float height;
	float near;
	float far;

public:
	Projection(ProjectionType pProjection, float pWidth, float pHeight, float pNear, float pFar);
	~Projection() {}

	void Update();

	inline mat4 GetProjection()
	{
		return projectionMatrix;
	}

	inline float GetWidth()
	{
		return width;
	}

	inline float GetHeight()
	{
		return height;
	}

	inline void SetWidth(float pWidth)
	{
		width = pWidth;
	}

	inline void SetHeight(float pHeight)
	{
		height = pHeight;
	}

	inline void SetNear(float pNear)
	{
		near = pNear;
	}

	inline void SetFar(float pFar)
	{
		far = pFar;
	}
};
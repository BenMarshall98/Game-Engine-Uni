#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

class Camera
{
private:
	mat4 viewMatrix;
	vec3 position;
	vec3 lookAt;
	vec3 up;
public:
	Camera(vec3 pPosition = vec3(0, 0, 0), vec3 pLookAt = vec3(0, 0, -1), vec3 pUp = vec3(0, 1, 0)) : position(pPosition), lookAt(pLookAt), up(pUp)
	{
		Update();
	}

	inline void Update()
	{
		viewMatrix = glm::lookAt(position, position + lookAt, up);
	}

	inline mat4 GetViewMatrix()
	{
		Update(); //TODO: Remove this and update in the scene itself
		return viewMatrix;
	}

	inline vec3 GetPosition()
	{
		return position;
	}

	inline vec3 GetLookAt()
	{
		return lookAt;
	}

	inline vec3 GetUp()
	{
		return up;
	}

	inline void SetPosition(vec3 pPosition)
	{
		position = pPosition;
	}

	inline void SetLookAt(vec3 pLookAt)
	{
		lookAt = pLookAt;
	}

	inline void SetUp(vec3 pUp)
	{
		up = pUp;
	}

	~Camera() {}
};


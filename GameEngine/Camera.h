#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

class Camera
{
private:
	mat4 renderViewMatrix;
	mat4 updateViewMatrix;

protected:
	vec3 position;
	vec3 lookAt;
	vec3 up;

public:
	explicit Camera(vec3 pPosition = vec3(0, 0, 0), vec3 pLookAt = vec3(0, 0, -1), vec3 pUp = vec3(0, 1, 0)) : position(pPosition), lookAt(pLookAt), up(pUp)
	{
	}

	virtual void Update()
	{
		updateViewMatrix = glm::lookAt(position, position + lookAt, up);
	}

	inline void Swap()
	{
		renderViewMatrix = updateViewMatrix;
	}

	inline mat4 GetViewMatrix()
	{
		return renderViewMatrix;
	}

	inline vec3 GetPosition() const
	{
		return position;
	}

	inline vec3 GetLookAt() const
	{
		return lookAt;
	}

	inline vec3 GetUp() const
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

	virtual ~Camera() {}
};


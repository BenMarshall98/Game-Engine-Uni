#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <map>
#include <vector>
#include "InputMapping.h"

using namespace glm;

class Camera
{
	using CameraFunction = void(*)(Camera *, float, float);

private:
	mat4 renderViewMatrix;
	mat4 updateViewMatrix;
	vec3 position;
	vec3 lookAt;
	vec3 up;

	map<CameraFunction, vector<GameInput>> * mCameraFunctions;

public:
	explicit Camera(map<CameraFunction, vector<GameInput>> * const pCameraFunctions) :position(vec3(0, 0, 0)), lookAt(vec3(0, 0, -1)), up(vec3(0, 1, 0)), mCameraFunctions(pCameraFunctions)
	{
	}

	explicit Camera(map<CameraFunction, vector<GameInput>> * const pCameraFunctions, vec3 & pPosition, vec3 & pLookAt, vec3 & pUp) : position(pPosition), lookAt(pLookAt), up(pUp), mCameraFunctions(pCameraFunctions)
	{
	}

	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	virtual void Update();

	inline void Swap()
	{
		renderViewMatrix = updateViewMatrix;
	}

	inline const mat4 & GetViewMatrix() const
	{
		return mat4(renderViewMatrix);
	}

	inline const vec3 & GetPosition() const
	{
		return vec3(position);
	}

	inline const vec3 & GetLookAt() const
	{
		return vec3(lookAt);
	}

	inline const vec3 & GetUp() const
	{
		return vec3(up);
	}

	inline void SetPosition(const vec3 & pPosition)
	{
		position = pPosition;
	}

	inline void SetLookAt(const vec3 & pLookAt)
	{
		lookAt = pLookAt;
	}

	inline void SetUp(const vec3 & pUp)
	{
		up = pUp;
	}

	virtual ~Camera();
};


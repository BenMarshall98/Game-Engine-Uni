#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <map>
#include <vector>
#include "InputMapping.h"

class Camera
{
	using CameraFunction = void(*)(Camera *, float, float);

private:
	glm::mat4 renderViewMatrix;
	glm::mat4 updateViewMatrix;
	glm::vec3 position;
	glm::vec3 lookAt;
	glm::vec3 up;

	std::map<CameraFunction, std::vector<GameInput>> * mCameraFunctions;

public:
	explicit Camera(std::map<CameraFunction, std::vector<GameInput>> * const pCameraFunctions) :position(glm::vec3(0, 0, 0)), lookAt(glm::vec3(0, 0, -1)), up(glm::vec3(0, 1, 0)), mCameraFunctions(pCameraFunctions)
	{
	}

	explicit Camera(std::map<CameraFunction, std::vector<GameInput>> * const pCameraFunctions, glm::vec3 & pPosition, glm::vec3 & pLookAt, glm::vec3 & pUp) : position(pPosition), lookAt(pLookAt), up(pUp), mCameraFunctions(pCameraFunctions)
	{
	}

	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	virtual void Update();

	inline void Swap()
	{
		renderViewMatrix = updateViewMatrix;
	}

	inline const glm::mat4 & GetViewMatrix() const
	{
		return renderViewMatrix;
	}

	inline const glm::vec3 & GetPosition() const
	{
		return position;
	}

	inline const glm::vec3 & GetLookAt() const
	{
		return lookAt;
	}

	inline const glm::vec3 & GetUp() const
	{
		return up;
	}

	inline void SetPosition(const glm::vec3 & pPosition)
	{
		position = pPosition;
	}

	inline void SetLookAt(const glm::vec3 & pLookAt)
	{
		lookAt = pLookAt;
	}

	inline void SetUp(const glm::vec3 & pUp)
	{
		up = pUp;
	}

	virtual ~Camera();
};


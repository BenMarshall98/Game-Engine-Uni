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

	//Keeps the camera concurrency compilant
	inline void Swap()
	{
		renderViewMatrix = updateViewMatrix;
	}

	//Gets the camera matrix
	inline const glm::mat4 & GetViewMatrix() const
	{
		return renderViewMatrix;
	}

	//Gets the camera position
	inline const glm::vec3 & GetPosition() const
	{
		return position;
	}

	//Gets the camera look at direction
	inline const glm::vec3 & GetLookAt() const
	{
		return lookAt;
	}

	//Gets the camera up direction
	inline const glm::vec3 & GetUp() const
	{
		return up;
	}

	//Sets the camera position
	inline void SetPosition(const glm::vec3 & pPosition)
	{
		position = pPosition;
	}

	//Sets the camera look at direction
	inline void SetLookAt(const glm::vec3 & pLookAt)
	{
		lookAt = pLookAt;
	}

	//Sets the camera up direction
	inline void SetUp(const glm::vec3 & pUp)
	{
		up = pUp;
	}

	virtual ~Camera();
};


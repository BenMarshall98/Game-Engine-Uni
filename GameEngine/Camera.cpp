#include "Camera.h"

Camera::~Camera()
{

}

//Updates the camera matrix
void Camera::Update()
{
	updateViewMatrix = glm::lookAt(position, position + lookAt, up);
}
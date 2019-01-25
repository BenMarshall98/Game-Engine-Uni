#include "Camera.h"

Camera::~Camera()
{

}

void Camera::Update()
{
	updateViewMatrix = glm::lookAt(position, position + lookAt, up);
}
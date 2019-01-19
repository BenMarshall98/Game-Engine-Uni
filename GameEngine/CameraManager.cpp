#include "CameraManager.h"

void CameraManager::SetCamera(Camera * pCamera)
{
	camera = pCamera;
}

void CameraManager::SetProjection(Projection * pProjection)
{
	projection = pProjection;
}

Camera * CameraManager::GetCamera()
{
	return camera;
}

Projection * CameraManager::GetProjection()
{
	return projection;
}
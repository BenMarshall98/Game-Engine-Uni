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

void CameraManager::Update()
{
	camera->Update();
}

void CameraManager::Resize(float width, float height)
{
	projection->SetHeight(height);
	projection->SetWidth(width);
	projection->Update();
}
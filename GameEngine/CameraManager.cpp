#include "CameraManager.h"
#include "InputManager.h"

CameraManager * CameraManager::instance = nullptr;

void CameraManager::Resize(const float width, const float height)
{
	projection->SetHeight(height);
	projection->SetWidth(width);
	projection->Update();
}
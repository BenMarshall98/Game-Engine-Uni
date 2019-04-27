#include "CameraManager.h"
#include "InputManager.h"

CameraManager * CameraManager::instance = nullptr;

//Makes sure the projection to updated with the new screen size
void CameraManager::Resize(const float width, const float height)
{
	if (projection)
	{
		projection->SetHeight(height);
		projection->SetWidth(width);
		projection->Update();
	}
}
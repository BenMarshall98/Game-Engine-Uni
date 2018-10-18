#include "GLFWWindow.h"
#include "SceneManager.h"

int main(void)
{
	SceneManager& sceneManager = SceneManager::instance();

	sceneManager.SetWindow(new GLFWWindow());
	sceneManager.Run();

	return 0;
}
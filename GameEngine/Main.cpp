#include "GLFWWindow.h"
#include "SceneManager.h"
#include "TestGameScene.h"

int main(void)
{
	SceneManager& sceneManager = SceneManager::instance();

	sceneManager.SetWindow(new GLFWWindow());
	sceneManager.SetScene(new TestGameScene());
	sceneManager.Run();

	return 0;
}
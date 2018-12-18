#include "GLFWWindow.h"
#include "SceneManager.h"
#include "TestGameScene.h"

int main(void)
{
	SceneManager * sceneManager = SceneManager::Instance();

	sceneManager->SetWindow(new GLFWWindow());
	sceneManager->SetScene(new TestGameScene());
	sceneManager->Run();

	delete sceneManager;

	return 0;
}
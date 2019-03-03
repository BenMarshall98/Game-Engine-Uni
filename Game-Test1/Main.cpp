#include "GLFWWindow.h"
#include "SceneManager.h"
#include "TestGameScene.h"
#include "MenuScene.h"

int main(void)
{
	SceneManager * const sceneManager = SceneManager::Instance();

	sceneManager->SetWindow(new GLFWWindow());
	sceneManager->SetScene(new MenuScene());
	sceneManager->Run();

	delete sceneManager;

	return 0;
}
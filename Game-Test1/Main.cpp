#include "GLFWWindow.h"
#include "SceneManager.h"
#include "MenuScene.h"

int main(void)
{
	SceneManager * const sceneManager = SceneManager::Instance();

	sceneManager->SetWindow(new GLFWWindow());
	sceneManager->NewScene(new MenuScene("GameChoosing.json"));
	sceneManager->Run();

	delete sceneManager;

	return 0;
}
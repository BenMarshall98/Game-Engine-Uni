#include "GLFWWindow.h"
#include "SceneManager.h"
#include "MenuScene.h" //or "GameScene.h"

int main(void)
{
	SceneManager * const sceneManager = SceneManager::Instance();
	//Optional: Used to set up the engine
	sceneManager->ConfigEngine("EngineConfig.json");
	
	//The first scene in the game
	sceneManager->NewScene(new MenuScene("GameChoosing.json"));
	
	//Starts the game engine running
	sceneManager->Run(); 

	delete sceneManager;

	return 0;
}
#include "SceneManager.h"


SceneManager::~SceneManager()
{
	delete currentScene;
}

void SceneManager::Run()
{

}

void SceneManager::SetScene(iScene * scene)
{
	delete currentScene;
	currentScene = scene;
}

SceneManager& SceneManager::instance()
{
	static SceneManager *instance = new SceneManager();
	return *instance;
}

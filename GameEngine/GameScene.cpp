#define GLM_ENABLE_EXPERIMENTAL

#include "OpenGL.h"
#include "GLFWWindow.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "ResourceManager.h"
#include "LightManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "CameraManager.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "PhysicsManager.h"
#include "ResourceManager.h"
#include "glm/gtx/transform.hpp"
#include "CameraManager.h"
#include "AudioManager.h"
#include "BulletPhysicsEngine.h"
#include "LevelLoader.h"
#include <iostream>

GameScene::GameScene(const std::string & pFileName) : iScene(pFileName)
{
}

void GameScene::Load()
{
	LevelLoader::LoadLevelJSON(fileName);

	PhysicsManager * const physicsManager = PhysicsManager::Instance();
	physicsManager->SetPhysicsEngine(new BulletPhysicsEngine());
}

void GameScene::Render()
{
	glEnable(GL_DEPTH_TEST);

	LightManager * const lightManager = LightManager::Instance();
	lightManager->Update(CameraManager::Instance()->GetCamera()->GetPosition());

	SystemManager::Instance()->Render();

	EntityManager::Instance()->Update();
}

void GameScene::Update()
{
	static int delay = 0;

	if (delay == 0)
	{
		const float inputValue = InputManager::Instance()->GetInputValue(escapeMenu);

		if (inputValue > 0.2)
		{
			SceneManager::Instance()->NewScene(new MenuScene("Pause.json"));
			delay = 10;
		}
	}
	else
	{
		delay--;
	}
	

	SystemManager::Instance()->Update();
	
	CameraManager::Instance()->Update();
	AudioManager::Instance()->Update();
}

void GameScene::Close()
{
	SystemManager::Instance()->Clear();
	EntityManager::Instance()->Clear();
	PhysicsManager::Instance()->Clear();
	AudioManager::Instance()->Clear();
	CameraManager::Instance()->Clear();
	LightManager::Instance()->Clear();
	ResourceManager::ClearResources();
}

void GameScene::Swap()
{

}

GameScene::~GameScene()
{
}
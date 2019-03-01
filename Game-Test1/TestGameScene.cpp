#define GLM_ENABLE_EXPERIMENTAL

#include "OpenGL.h"
#include "GLFWWindow.h"
#include "TestGameScene.h"
#include "ResourceManager.h"
#include "LightManager.h"
#include "InputManager.h"
#include "glm/gtx/transform.hpp"
#include "CameraManager.h"
#include "AudioManager.h"
#include "BulletPhysicsEngine.h"
#include "LevelLoader.h"
#include <iostream>

TestGameScene::TestGameScene() : mSystemManager(SystemManager::Instance()), mPhysicsManager(nullptr)
{
}

void TestGameScene::Load()
{
	cout << "Please pick a level:" << endl;
	cout << "   1. 3DLevel" << endl;
	cout << "   2. PlatformLevel" << endl;

	unsigned int level = 0;

	while (level > 2 || level == 0)
	{
		cin >> level;
	}

	string levelName = "Levels/3DLevel.json";

	if (level == 2)
	{
		levelName = "Levels/PlatformLevel.json";
	}
	LevelLoader::LoadLevelJSON(levelName);

	mPhysicsManager = PhysicsManager::Instance();
	mPhysicsManager->SetPhysicsEngine(new BulletPhysicsEngine());
}

void TestGameScene::Render()
{
	glEnable(GL_DEPTH_TEST);

	LightManager * const lightManager = LightManager::Instance();
	lightManager->Update(CameraManager::Instance()->GetCamera()->GetPosition());

	mSystemManager->Render();

	EntityManager::Instance()->Update();
}

void TestGameScene::Update()
{
	mSystemManager->Update();
	
	CameraManager::Instance()->Update();
	AudioManager::Instance()->Update();
}

void TestGameScene::Close()
{
}

void TestGameScene::Resize(const int width, const int height)
{
}

TestGameScene::~TestGameScene()
{
}
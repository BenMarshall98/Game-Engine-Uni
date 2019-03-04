#define GLM_ENABLE_EXPERIMENTAL

#include "OpenGL.h"
#include "GLFWWindow.h"
#include "GameScene.h"
#include "ResourceManager.h"
#include "LightManager.h"
#include "InputManager.h"
#include "glm/gtx/transform.hpp"
#include "CameraManager.h"
#include "AudioManager.h"
#include "BulletPhysicsEngine.h"
#include "LevelLoader.h"
#include <iostream>

GameScene::GameScene(std::string pFileName) : iScene(pFileName), mSystemManager(SystemManager::Instance()), mPhysicsManager(nullptr)
{
}

void GameScene::Load()
{
	LevelLoader::LoadLevelJSON(fileName);

	mPhysicsManager = PhysicsManager::Instance();
	mPhysicsManager->SetPhysicsEngine(new BulletPhysicsEngine());
}

void GameScene::Render()
{
	glEnable(GL_DEPTH_TEST);

	LightManager * const lightManager = LightManager::Instance();
	lightManager->Update(CameraManager::Instance()->GetCamera()->GetPosition());

	mSystemManager->Render();

	EntityManager::Instance()->Update();
}

void GameScene::Update()
{
	mSystemManager->Update();
	
	CameraManager::Instance()->Update();
	AudioManager::Instance()->Update();
}

void GameScene::Close()
{
}

void GameScene::Swap()
{

}

GameScene::~GameScene()
{
}
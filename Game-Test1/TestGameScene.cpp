#define GLM_ENABLE_EXPERIMENTAL

#include "OpenGL.h"
#include "GLFWWindow.h"
#include "TestGameScene.h"
#include "ResourceManager.h"
#include "ComponentModel.h"
#include "ComponentPosition.h"
#include "ComponentShader.h"
#include "ComponentTexture.h"
#include "ComponentNormalTexture.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "LightManager.h"
#include "InputManager.h"
#include "glm/gtx/transform.hpp"
#include "ComponentInput.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"
#include "CollisionCuboid.h"
#include "FollowPlaneCamera.h"
#include "CameraManager.h"

#include "BulletPhysicsEngine.h"
#include "PhysicsSystem.h"
#include "LevelLoader.h"

TestGameScene::TestGameScene() : mPhysicsManager(nullptr)
{
}

void TestGameScene::Load()
{

	LevelLoader::LoadLevelJSON("3DLevel.json");

	mPhysicsManager = new PhysicsManager(new BulletPhysicsEngine());

	RenderSystem * const render = new RenderSystem();
	mSystemManager.AddRenderSystem(render);

	InputSystem * const input = new InputSystem();
	mSystemManager.AddUpdateSystem(input);

	PhysicsSystem * const physics = new PhysicsSystem(*mPhysicsManager);
	mSystemManager.AddUpdateSystem(physics);

	LightManager * const lightManager = LightManager::Instance();
	lightManager->SetDirectionalLight(vec3(0, -1, 0), vec3(1.0, 1.0, 1.0));
	lightManager->AddPointLight(vec3(0, 0, -3.0f), vec3(1, 1, 1), 0.1f);
	lightManager->AddSpotLight(vec3(0.5f, 0.5f, -1.0f), vec3(0, 0, -1), vec3(1.0, 1.0, 1.0), 2.5f, 5);
	lightManager->AddSpotLight(vec3(0.5f, -0.5f, -1.0f), vec3(0, 0, -1), vec3(0.0, 0.0, 1.0), 2.5f, 5);
	lightManager->AddSpotLight(vec3(-0.5f, -0.5f, -1.0f), vec3(0, 0, -1), vec3(0.0, 1.0, 0.0), 2.5f, 5);
	lightManager->AddSpotLight(vec3(-0.5f, 0.5f, -1.0f), vec3(0, 0, -1), vec3(1.0, 0.0, 0.0), 2.5f, 5);
}

void TestGameScene::Render()
{
	glEnable(GL_DEPTH_TEST);

	mSystemManager.Render();

	EntityManager::Instance()->Update(mSystemManager);
}

void TestGameScene::Update()
{
	mSystemManager.Update();
	
	CameraManager::Instance()->Update();
}

void TestGameScene::Close()
{
}

void TestGameScene::Resize(const int width, const int height)
{
}

TestGameScene::~TestGameScene()
{
	delete mPhysicsManager;
}
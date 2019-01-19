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

#include "BulletPhysicsEngine.h"
#include "PhysicsSystem.h"
#include "LevelLoader.h"

TestGameScene::TestGameScene() : mPhysicsManager(nullptr), camera(nullptr), projection(nullptr)
{
}

void TestGameScene::Load()
{
	EntityManager * mEntityManager = EntityManager::Instance();

	projection = new Projection(Perspective, GLFWWindow::GetWidth(), GLFWWindow::GetHeight(), 0.1f, 100.0f);

	LevelLoader::LoadLevelJSON("Level.json");

	Entity * entity = mEntityManager->GetEntityByName("Player");

	camera = new FollowPlaneCamera(entity, XY, 2, 50, 4, 0.1);

	mPhysicsManager = new PhysicsManager(new BulletPhysicsEngine());

	RenderSystem * render = new RenderSystem(camera, projection);
	mSystemManager.AddRenderSystem(render);

	InputSystem * input = new InputSystem();
	mSystemManager.AddUpdateSystem(input); //TODO: change to a update system

	PhysicsSystem * physics = new PhysicsSystem(*mPhysicsManager);
	mSystemManager.AddUpdateSystem(physics);

	LightManager * lightManager = LightManager::Instance();
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
	camera->Update();
}

void TestGameScene::Close()
{
	delete camera;
}

void TestGameScene::Resize(int width, int height)
{
	projection->SetHeight(height);
	projection->SetWidth(width);
}

TestGameScene::~TestGameScene()
{
	delete mPhysicsManager;
	delete camera;
	delete projection;
}
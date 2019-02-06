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
#include "ShadowSystem.h"
#include "LightManager.h"
#include "InputManager.h"
#include "glm/gtx/transform.hpp"
#include "ComponentInput.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"
#include "CollisionCuboid.h"
#include "FollowPlaneCamera.h"
#include "CameraManager.h"
#include "AudioManager.h"
#include "AudioSystem.h"

#include "BulletPhysicsEngine.h"
#include "PhysicsSystem.h"
#include "LevelLoader.h"
#include <iostream>

TestGameScene::TestGameScene() : mPhysicsManager(nullptr)
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

	mPhysicsManager = new PhysicsManager(new BulletPhysicsEngine());

	ResourceManager::LoadShader("DirectionalShadow", "Shaders/ShadowMapping.vert", "Shaders/ShadowMapping.frag");
	ResourceManager::LoadShader("PointShadow", "Shaders/PointShadow.vert", "Shaders/PointShadow.frag", "Shaders/PointShadow.geom");
	Shader * directionalShadow = ResourceManager::GetShader("DirectionalShadow");
	Shader * pointLightShadow = ResourceManager::GetShader("PointShadow");

	vec3 topLeftCoord = vec3(-15, 15, 15);
	vec3 bottomRightCoord = vec3(15, -15, -15);

	ShadowSystem * const shadow = new ShadowSystem(topLeftCoord, bottomRightCoord, directionalShadow, pointLightShadow);
	mSystemManager.AddRenderSystem(shadow);

	RenderSystem * const render = new RenderSystem();
	mSystemManager.AddRenderSystem(render);

	InputSystem * const input = new InputSystem();
	mSystemManager.AddUpdateSystem(input);

	PhysicsSystem * const physics = new PhysicsSystem(*mPhysicsManager);
	mSystemManager.AddUpdateSystem(physics);

	AudioSystem * const audio = new AudioSystem();
	mSystemManager.AddUpdateSystem(audio);
}

void TestGameScene::Render()
{
	glEnable(GL_DEPTH_TEST);

	LightManager * lightManager = LightManager::Instance();
	lightManager->Update(CameraManager::Instance()->GetCamera()->GetPosition());

	mSystemManager.Render();

	EntityManager::Instance()->Update(mSystemManager);
}

void TestGameScene::Update()
{
	mSystemManager.Update();
	
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
	delete mPhysicsManager;
}
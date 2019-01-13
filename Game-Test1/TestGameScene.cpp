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
#include "ScriptingManager.h"

TestGameScene::TestGameScene()
{
}


TestGameScene::~TestGameScene()
{
}

void TestGameScene::Load()
{
	ScriptingManager * scriptingManager = ScriptingManager::Instance();

	

	mEntityManager = EntityManager::Instance();
	InputManager * inputManager = InputManager::Instance();
	GLFWInput * inputReader = inputManager->GetInputReader();

	inputReader->AllowGamePadInput(false);
	inputReader->AllowMouseInput(true);
	inputReader->AllowKeyboardInput(true);

	projection = new Projection(Perspective, GLFWWindow::width, GLFWWindow::height, 0.1f, 100.0f);

	InputFunction playerLeft = InputFunction(PlayerLeft);
	playerLeft.AddInput(KEYBOARD_A);
	playerLeft.AddInput(GAMEPAD_L_LEFT);

	InputFunction playerRight = InputFunction(PlayerRight);
	playerRight.AddInput(KEYBOARD_D);
	playerRight.AddInput(GAMEPAD_L_RIGHT);

	InputFunction playerJump = InputFunction(PlayerJump);
	playerJump.AddInput(KEYBOARD_SPACE);
	playerJump.AddInput(GAMEPAD_X);

	vector<InputFunction> * playerInputs = new vector<InputFunction>;
	playerInputs->push_back(playerLeft);
	playerInputs->push_back(playerRight);
	playerInputs->push_back(playerJump);

	LevelLoader::LoadLevelJSON("Level.json");
	//LevelLoader::LoadLevel("Level.txt");

	Entity * entity = mEntityManager->GetEntityByName("Player");
	mEntityManager->AddComponentToEntity(entity, new ComponentInput(playerInputs));

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

	cameraLeftInputs.push_back(KEYBOARD_LEFT);
	cameraLeftInputs.push_back(MOUSE_LEFT);
	cameraLeftInputs.push_back(GAMEPAD_LEFT);
	cameraLeftInputs.push_back(GAMEPAD_LT);

	cameraRightInputs.push_back(KEYBOARD_RIGHT);
	cameraRightInputs.push_back(MOUSE_RIGHT);
	cameraRightInputs.push_back(GAMEPAD_RIGHT);
	cameraRightInputs.push_back(GAMEPAD_RT);

	cameraUpInputs.push_back(KEYBOARD_UP);
	cameraUpInputs.push_back(MOUSE_UP);
	cameraUpInputs.push_back(GAMEPAD_UP);
	cameraUpInputs.push_back(GAMEPAD_LB);

	cameraDownInputs.push_back(KEYBOARD_DOWN);
	cameraDownInputs.push_back(MOUSE_DOWN);
	cameraDownInputs.push_back(GAMEPAD_DOWN);
	cameraDownInputs.push_back(GAMEPAD_RB);

	Entity * pEntity = mEntityManager->GetEntityByName("Collectable");

	scriptingManager->LoadLuaFromFile("../GameEngine/Vector3.lua");
	scriptingManager->LoadLuaFromFile("TestFunctions.lua");
	scriptingManager->RunScriptFromFunction("TestFunction", pEntity);
}

void TestGameScene::Render()
{
	glEnable(GL_DEPTH_TEST);

	//camera->Update();
	//UpdateCamera();

	mSystemManager.Render();

	mEntityManager->Update(mSystemManager);
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

void TestGameScene::PlayerLeft(float value, Entity * entity)
{
	if (value > 0.2f)
	{
		iComponent * componentPhysics = EntityManager::Instance()->GetComponentOfEntity(entity, COMPONENT_PHYSICS);
		
		vec3 velocity = ((ComponentPhysics *)componentPhysics)->GetUpdateVelocity();
		velocity.x -= ((1 / 60.0f) * value * 1000);
		((ComponentPhysics *)componentPhysics)->SetUpdateVelocity(velocity);
	}
}

void TestGameScene::PlayerRight(float value, Entity * entity)
{
	if (value > 0.2f)
	{
		iComponent * componentPhysics = EntityManager::Instance()->GetComponentOfEntity(entity, COMPONENT_PHYSICS);

		vec3 velocity = ((ComponentPhysics *)componentPhysics)->GetUpdateVelocity();
		velocity.x += ((1 / 60.0f) * value * 1000);
		((ComponentPhysics *)componentPhysics)->SetUpdateVelocity(velocity);
	}
}

void TestGameScene::PlayerJump(float value, Entity * entity)
{
	if (value > 0.2f)
	{
		iComponent * componentPhysics = EntityManager::Instance()->GetComponentOfEntity(entity, COMPONENT_PHYSICS);

		if (((ComponentPhysics *)componentPhysics)->GetUpdateTouchingGround())
		{
 			vec3 impulse = ((ComponentPhysics *)componentPhysics)->GetUpdateImpulse();
			impulse.y += ((1 / 60.0f) * value * 100);
			((ComponentPhysics *)componentPhysics)->SetUpdateImpulse(impulse);
		}
	}
}
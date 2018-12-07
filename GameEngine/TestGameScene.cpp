#define GLM_ENABLE_EXPERIMENTAL

#include "OpenGL.h"
#include "GLFWWindow.h"
#include "TestGameScene.h"
#include "ResourceManager.h"
#include "ComponentModel.h"
#include "ComponentPosition.h"
#include "ComponentShader.h"
#include "ComponentTexture.h"
#include "ComponentNormal.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "LightManager.h"
#include "InputManager.h"
#include "glm/gtx/transform.hpp"
#include "ComponentInput.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"
#include "CollisionCuboid.h"

#include "BulletPhysicsEngine.h"
#include "PhysicsSystem.h"
#include "LevelLoader.h"

TestGameScene::TestGameScene()
{
}


TestGameScene::~TestGameScene()
{
}

void TestGameScene::Load()
{
	InputManager * inputManager = InputManager::Instance();
	GLFWInput * inputReader = inputManager->GetInputReader();

	inputReader->GamePadInput(false);
	inputReader->MouseInput(true);
	inputReader->KeyboardInput(true);

	camera = new Camera();
	projection = new Projection(ProjectionType::Perspective, GLFWWindow::width, GLFWWindow::height, 0.1f, 100.0f);

	ResourceManager::LoadModel("Cube", "cube.obj");
	ResourceManager::LoadModel("Sphere", "sphere.obj");
	//ResourceManager::LoadModel("Test", "Monster_1.dae");
	ResourceManager::LoadShader("TestShader", "TestVertex.vert", "TestFragment.frag");
	ResourceManager::LoadShader("NormalShader", "NormalVertex.vert", "NormalFragment.frag");
	ResourceManager::LoadShader("RiggedShader", "RiggedVertex.vert", "RiggedFragment.frag");
	ResourceManager::LoadTexture("Box", "container.jpg");
	ResourceManager::LoadTexture("BoxNormal", "containerNormal.jpg");

	InputFunction cubeOneLeft = InputFunction(CubeLeft);
	cubeOneLeft.AddInput(KEYBOARD_A);
	cubeOneLeft.AddInput(GAMEPAD_L_LEFT);

	InputFunction cubeOneRight = InputFunction(CubeRight);
	cubeOneRight.AddInput(KEYBOARD_D);
	cubeOneRight.AddInput(GAMEPAD_L_RIGHT);

	InputFunction cubeOneUp = InputFunction(CubeUp);
	cubeOneUp.AddInput(KEYBOARD_W);
	cubeOneUp.AddInput(GAMEPAD_L_UP);

	vector<InputFunction> cubeOneInputs;
	cubeOneInputs.push_back(cubeOneLeft);
	cubeOneInputs.push_back(cubeOneRight);
	cubeOneInputs.push_back(cubeOneUp);

	InputFunction cubeTwoLeft = InputFunction(CubeLeft);
	cubeTwoLeft.AddInput(KEYBOARD_H);
	cubeTwoLeft.AddInput(GAMEPAD_R_LEFT);

	InputFunction cubeTwoRight = InputFunction(CubeRight);
	cubeTwoRight.AddInput(KEYBOARD_K);
	cubeTwoRight.AddInput(GAMEPAD_R_RIGHT);

	InputFunction cubeTwoUp = InputFunction(CubeUp);
	cubeTwoUp.AddInput(KEYBOARD_U);
	cubeTwoUp.AddInput(GAMEPAD_R_UP);

	vector<InputFunction> cubeTwoInputs;
	cubeTwoInputs.push_back(cubeTwoLeft);
	cubeTwoInputs.push_back(cubeTwoRight);
	cubeTwoInputs.push_back(cubeTwoUp);

	LevelLoader::LoadLevel("Level.txt", mEntityManager);

	mPhysicsManager = new PhysicsManager(new BulletPhysicsEngine());

	RenderSystem * render = new RenderSystem(mEntityManager, camera, projection);
	mSystemManager.AddRenderSystem(render);

	InputSystem * input = new InputSystem(mEntityManager);
	mSystemManager.AddRenderSystem(input); //TODO: change to a update system

	PhysicsSystem * physics = new PhysicsSystem(mEntityManager, *mPhysicsManager);
	mSystemManager.AddRenderSystem(physics);

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
}

void TestGameScene::Render()
{
	glEnable(GL_DEPTH_TEST);

	UpdateCamera();

	mSystemManager.Render();
}

void TestGameScene::Update()
{

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

void TestGameScene::UpdateCamera()
{
	const float angle = 30 * (1 / 60.0f); //TODO: replace
	InputManager * inputManager = InputManager::Instance();

	float leftValue = inputManager->GetInputValue(cameraLeftInputs);
	float rightValue = inputManager->GetInputValue(cameraRightInputs);
	float upValue = inputManager->GetInputValue(cameraUpInputs);
	float downValue = inputManager->GetInputValue(cameraDownInputs);

	if (leftValue > 0.2)
	{
		vec3 view = camera->GetLookAt();
		view = view * mat3(rotate(radians(-angle * leftValue), vec3(0, 1, 0)));
		camera->SetLookAt(view);
	}

	if (rightValue > 0.2)
	{
		vec3 view = camera->GetLookAt();
		view = view * mat3(rotate(radians(angle * rightValue), vec3(0, 1, 0)));
		camera->SetLookAt(view);
	}

	if (upValue > 0.2)
	{
		vec3 view = camera->GetLookAt();
		view = view * mat3(rotate(radians(-angle * upValue), vec3(1, 0, 0)));
		camera->SetLookAt(view);
	}

	if (downValue > 0.2)
	{
		vec3 view = camera->GetLookAt();
		view = view * mat3(rotate(radians(angle * downValue), vec3(1, 0, 0)));
		camera->SetLookAt(view);
	}
}

void TestGameScene::CubeLeft(float value, Entity * entity)
{
	if (value > 0.2f)
	{
		iComponent * componentPhysics = mEntityManager.GetComponentOfEntity(entity, COMPONENT_PHYSICS);
		
		vec3 force = ((ComponentPhysics *)componentPhysics)->GetForce();
		force.x -= ((1 / 60.0f) * value * 100);
		((ComponentPhysics *)componentPhysics)->SetForce(force);
	}
}

void TestGameScene::CubeRight(float value, Entity * entity)
{
	if (value > 0.2f)
	{
		iComponent * componentPhysics = mEntityManager.GetComponentOfEntity(entity, COMPONENT_PHYSICS);

		vec3 force = ((ComponentPhysics *)componentPhysics)->GetForce();
		force.x += ((1 / 60.0f) * value * 100);
		((ComponentPhysics *)componentPhysics)->SetForce(force);
	}
}

void TestGameScene::CubeUp(float value, Entity * entity)
{
	if (value > 0.2f)
	{
		iComponent * componentPhysics = mEntityManager.GetComponentOfEntity(entity, COMPONENT_PHYSICS);

		vec3 impulse = ((ComponentPhysics *)componentPhysics)->GetImpulse();
		impulse.y += ((1 / 60.0f) * value * 10);
		((ComponentPhysics *)componentPhysics)->SetImpulse(impulse);
	}
}

EntityManager TestGameScene::mEntityManager = EntityManager();
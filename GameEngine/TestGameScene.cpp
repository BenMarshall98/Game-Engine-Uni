#include "OpenGL.h"
#include "GLFWWindow.h"
#include "TestGameScene.h"
#include "ResourceManager.h"
#include "ComponentModel.h"
#include "ComponentPosition.h"
#include "ComponentShader.h"
#include "ComponentTexture.h"
#include "RenderSystem.h"
#include "LightManager.h"

TestGameScene::TestGameScene()
{
}


TestGameScene::~TestGameScene()
{
}

void TestGameScene::Load()
{
	camera = new Camera();
	projection = new Projection(ProjectionType::Perspective, GLFWWindow::width, GLFWWindow::height, 0.1f, 100.0f);

	ResourceManager::LoadModel("Cube", "cube.obj");
	//ResourceManager::LoadModel("Test", "Monster_1.dae");
	ResourceManager::LoadShader("TestShader", "TestVertex.vert", "TestFragment.frag");
	ResourceManager::LoadShader("RiggedShader", "RiggedVertex.vert", "RiggedFragment.frag");
	ResourceManager::LoadTexture("Box", "container.jpg");

	Entity * newEntity = mEntityManager.CreateEntity();
	mEntityManager.AddComponentToEntity(newEntity, new ComponentModel("Cube"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentShader("TestShader"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentPosition(vec3(0.5f, 0.5f, -3.0f)));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentTexture("Box"));

	newEntity = mEntityManager.CreateEntity();
	mEntityManager.AddComponentToEntity(newEntity, new ComponentModel("Cube"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentShader("TestShader"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentPosition(vec3(0.5f, -0.5f, -3.0f)));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentTexture("Box"));

	newEntity = mEntityManager.CreateEntity();
	mEntityManager.AddComponentToEntity(newEntity, new ComponentModel("Cube"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentShader("TestShader"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentPosition(vec3(-0.5f, -0.5f, -3.0f)));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentTexture("Box"));

	newEntity = mEntityManager.CreateEntity();
	mEntityManager.AddComponentToEntity(newEntity, new ComponentModel("Cube"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentShader("TestShader"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentPosition(vec3(-0.5f, 0.5f, -3.0f)));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentTexture("Box"));

	/*newEntity = mEntityManager.CreateEntity();
	mEntityManager.AddComponentToEntity(newEntity, new ComponentModel("Test"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentShader("RiggedShader"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentPosition(vec3(0, 0, -50.0f)));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentTexture("Box"));*/

	RenderSystem * render = new RenderSystem(mEntityManager, camera, projection);
	mSystemManager.AddRenderSystem(render);

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
}

void TestGameScene::Update()
{

}

void TestGameScene::Close()
{
	delete camera;
}
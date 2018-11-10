#include "TestGameScene.h"
#include "ResourceManager.h"
#include "ComponentModel.h"
#include "ComponentPosition.h"
#include "ComponentShader.h"
#include "ComponentTexture.h"
#include "RenderSystem.h"
#include "OpenGL.h"

TestGameScene::TestGameScene()
{
}


TestGameScene::~TestGameScene()
{
}

void TestGameScene::Load()
{
	camera = new Camera();
	ResourceManager::LoadModel("Cube", "cube.obj");
	ResourceManager::LoadModel("Test", "Monster_1.dae");
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

	newEntity = mEntityManager.CreateEntity();
	mEntityManager.AddComponentToEntity(newEntity, new ComponentModel("Test"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentShader("RiggedShader"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentPosition(vec3(0, 0, -50.0f)));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentTexture("Box"));

	RenderSystem * render = new RenderSystem(mEntityManager);
	mSystemManager.AddRenderSystem(render);
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
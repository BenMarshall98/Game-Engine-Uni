#include "TestGameScene.h"
#include "ResourceManager.h"
#include "ComponentModel.h"
#include "ComponentPosition.h"
#include "ComponentShader.h"
#include "RenderSystem.h"

TestGameScene::TestGameScene()
{
}


TestGameScene::~TestGameScene()
{
}

void TestGameScene::Load()
{
	ResourceManager::LoadModel("Cube", "cube.obj");
	ResourceManager::LoadShader("TestShader", "TestVertex.vert", "TestFragment.frag");

	Entity * newEntity = mEntityManager.CreateEntity();
	mEntityManager.AddComponentToEntity(newEntity, new ComponentModel("Cube"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentShader("TestShader"));
	mEntityManager.AddComponentToEntity(newEntity, new ComponentPosition(new vec3(0.0f, 0.0f, 0.0f)));

	RenderSystem * render = new RenderSystem(mEntityManager);
	mSystemManager.AddRenderSystem(render);
}

void TestGameScene::Render()
{
	mSystemManager.Render();
}

void TestGameScene::Update()
{

}

void TestGameScene::Close()
{

}
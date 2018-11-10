#pragma once

#include "iScene.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "Camera.h"

class TestGameScene : public iScene
{
private:
	EntityManager mEntityManager;
	SystemManager mSystemManager;
	Camera * camera;

public:
	TestGameScene();
	~TestGameScene();

	void Load();
	void Render();
	void Update();
	void Close();
};


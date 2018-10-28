#pragma once

#include "iScene.h"
#include "EntityManager.h"
#include "SystemManager.h"

class TestGameScene : public iScene
{
private:
	EntityManager mEntityManager;
	SystemManager mSystemManager;
public:
	TestGameScene();
	~TestGameScene();

	void Load();
	void Render();
	void Update();
	void Close();
};


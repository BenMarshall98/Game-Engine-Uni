#pragma once

#include "iScene.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "Camera.h"
#include "Projection.h"
#include "InputMapping.h"
#include "PhysicsManager.h"
#include <vector>

using namespace std;

class TestGameScene : public iScene
{
private:
	SystemManager mSystemManager;
	PhysicsManager * mPhysicsManager;
	Camera * camera;
	Projection * projection;

public:
	EntityManager * mEntityManager; //TODO: make into a singleton and make private

	TestGameScene();
	~TestGameScene();

	void Load();
	void Render();
	void Update();
	void Close();

	void Resize(int width, int height);
};


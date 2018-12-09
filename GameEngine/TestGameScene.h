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

enum EntityTypes
{
	PLAYER,
	WALL,
	COLLECTABLE
};

class TestGameScene : public iScene
{
private:
	
	static EntityManager mEntityManager; //TODO: make into a singleton
	SystemManager mSystemManager;
	PhysicsManager<EntityTypes> * mPhysicsManager;
	Camera * camera;
	Projection * projection;
	vector<GameInput> cameraLeftInputs;
	vector<GameInput> cameraRightInputs;
	vector<GameInput> cameraUpInputs;
	vector<GameInput> cameraDownInputs;

public:
	TestGameScene();
	~TestGameScene();

	void Load();
	void Render();
	void Update();
	void Close();

	void Resize(int width, int height);

	static void PlayerLeft(float value, Entity * entity);
	static void PlayerRight(float value, Entity * entity);
	static void PlayerJump(float value, Entity * entity);
};


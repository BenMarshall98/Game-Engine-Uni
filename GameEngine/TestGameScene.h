#pragma once

#include "iScene.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "Camera.h"
#include "Projection.h"
#include "InputMapping.h"
#include <vector>

using namespace std;

class TestGameScene : public iScene
{
private:
	static EntityManager mEntityManager; //TODO: make into a singleton
	SystemManager mSystemManager;
	Camera * camera;
	Projection * projection;
	vector<GameInput> cameraLeftInputs;
	vector<GameInput> cameraRightInputs;

public:
	TestGameScene();
	~TestGameScene();

	void Load();
	void Render();
	void Update();
	void Close();

	void UpdateCamera();

	static void CubeLeft(float value, Entity * entity);
	static void CubeRight(float value, Entity * entity);
};


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

public:

	TestGameScene();
	~TestGameScene();

	TestGameScene& operator=(const TestGameScene&) = delete;
	TestGameScene(TestGameScene&) = delete;

	void Load() override;
	void Render() override;
	void Update() override;
	void Close() override;

	void Resize(const int width, const int height) override;
};


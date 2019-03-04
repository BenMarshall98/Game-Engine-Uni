#pragma once

#include "iScene.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "Camera.h"
#include "Projection.h"
#include "InputMapping.h"
#include "PhysicsManager.h"
#include <vector>

class GameScene : public iScene
{
private:
	SystemManager * mSystemManager;
	PhysicsManager * mPhysicsManager;

public:

	GameScene(std::string pFileName);
	~GameScene();

	GameScene& operator=(const GameScene&) = delete;
	GameScene(GameScene&) = delete;

	void Load() override;
	void Render() override;
	void Update() override;
	void Close() override;
	void Swap() override;
};


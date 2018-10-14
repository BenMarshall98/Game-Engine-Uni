#pragma once
#include "iScene.h"

class SceneManager
{
private:
	iScene * currentScene;

	SceneManager() : currentScene(nullptr) {};

public:

	static SceneManager& instance();

	~SceneManager();

	void Run();
	void SetScene(iScene * scene);
};


#pragma once
#include "iScene.h"
#include "GLFWWindow.h"

using namespace std;

class SceneManager
{
private:
	iScene * currentScene;
	GLFWWindow * currentWindow;
	bool windowRunning;
	int updateCount = 0;
	int renderCount = 0;

	SceneManager() : currentScene(nullptr), currentWindow(nullptr) {};

	void Update();
	void Render();

public:

	static SceneManager& instance();

	~SceneManager();

	void Run();
	void SetScene(iScene * scene);
	void SetWindow(GLFWWindow * gameWindow);
};


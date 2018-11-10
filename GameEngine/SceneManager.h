#pragma once

#include <thread>

#include "iScene.h"
#include "GLFWWindow.h"
#include "OpenGL.h"

using namespace std;

class SceneManager
{
private:
	iScene * currentScene;
	iScene * newScene;
	GLFWWindow * currentWindow;
	bool windowRunning;
	bool sceneRunning;
	bool tempRunning;
	int updateCount = 0;
	int renderCount = 0;
	thread swap;

	SceneManager() : currentScene(nullptr), currentWindow(nullptr)
	{
	
	};

	void Update();
	void ThreadUpdate();
	void Render();
	void StartSwapScene(iScene * scene);
	void FinishSwapScene();

public:

	static SceneManager& instance();

	~SceneManager();

	void Run();
	void SetScene(iScene * scene);
	void SetWindow(GLFWWindow * gameWindow);
};


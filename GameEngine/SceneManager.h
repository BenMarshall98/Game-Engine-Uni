#pragma once

#include <thread>

#include "iScene.h"
#include "GLFWWindow.h"
#include "GLFWInput.h"
#include "LoggingManager.h"

using namespace std;

class SceneManager
{
private:
	static SceneManager * instance;

	iScene * currentScene;
	iScene * newScene;
	GLFWWindow * currentWindow;
	bool windowRunning;
	bool sceneRunning;
	bool tempRunning;
	int updateCount = 0;
	int renderCount = 0;
	thread swap;

	SceneManager() : currentScene(nullptr), newScene(nullptr), currentWindow(nullptr), windowRunning(true), sceneRunning(true), tempRunning(true)
	{
	};

	void Update();
	void Render();
	void StartSwapScene(iScene * scene);
	void FinishSwapScene();

public:

	static SceneManager * Instance();

	~SceneManager();

	void Run();
	void SetScene(iScene * scene);
	void SetWindow(GLFWWindow * gameWindow);
	void Resize(int width, int height);
};


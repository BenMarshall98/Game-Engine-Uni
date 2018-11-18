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
	LoggingManager * loggingManager;
	bool windowRunning;
	bool sceneRunning;
	bool tempRunning;
	int updateCount = 0;
	int renderCount = 0;
	thread swap;

	SceneManager() : currentScene(nullptr), currentWindow(nullptr)
	{
		loggingManager = LoggingManager::Instance();
	};

	void Update();
	void ThreadUpdate();
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


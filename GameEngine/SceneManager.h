#pragma once

#include <thread>

#include "iScene.h"
#include "Window.h"
#include "LoggingManager.h"

class SceneManager
{
private:
	static SceneManager * instance;
	std::thread swap;

	iScene * currentScene;
	iScene * newScene;
	Window * currentWindow;

	int updateCount = 0;
	int renderCount = 0;
	bool windowRunning;
	bool sceneRunning;
	bool tempRunning;

	SceneManager() : currentScene(nullptr), newScene(nullptr), currentWindow(nullptr), windowRunning(true), sceneRunning(true), tempRunning(true)
	{
	};

	inline void Update()
	{
		currentScene->Update();
	}

	inline void Render()
	{
		currentScene->Render();
	}

	void StartSwapScene(iScene * scene);
	void FinishSwapScene();

public:

	static SceneManager * Instance();

	SceneManager& operator= (const SceneManager&) = delete;
	SceneManager(SceneManager&) = delete;

	~SceneManager() throw();

	void Run();

	inline void SetScene(iScene * scene)
	{
		StartSwapScene(scene);

		if (currentScene == nullptr)
		{
			tempRunning = true;
			FinishSwapScene();
		}
	}

	inline void SetWindow(Window * gameWindow)
	{
		static bool firstTime = true;

		if (firstTime)
		{
			currentWindow = gameWindow;
			currentWindow->Load();
			firstTime = false;
		}
	}

	void Resize(int width, int height);
};


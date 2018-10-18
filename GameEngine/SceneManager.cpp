#include "OpenGL.h"
#include "SceneManager.h"
#include <iostream>
#include <thread>

SceneManager::~SceneManager()
{
	delete currentScene;
}

void SceneManager::Run()
{
	sceneRunning = true;
	unsigned int maxThreads = thread::hardware_concurrency();

	if (maxThreads < 2)
	{
		while (currentWindow->IsRunning() && windowRunning)
		{
			Update();
			Render();
		}
	}
	else
	{
		windowRunning = true;

		thread update = thread(&SceneManager::ThreadUpdate, this);

		while (currentWindow->IsRunning() && windowRunning)
		{
			Render();
		}

		windowRunning = false;
		update.join();
	}

	sceneRunning = false;
}

void SceneManager::Update()
{
	updateCount++;
}

void SceneManager::ThreadUpdate()
{
	while(windowRunning)
	{
		Update();
	}
}

void SceneManager::Render()
{	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	renderCount++;

	currentWindow->LimitFPS(60);
	currentWindow->WindowEvents();
}

void SceneManager::SetScene(iScene * scene)
{
	bool tempRunning = windowRunning;
	windowRunning = false;

	while (sceneRunning) {}

	currentScene->Close();
	delete currentScene;

	currentScene = scene;
	currentScene->Load();

	if (tempRunning)
	{
		Run();
	}
}

void SceneManager::SetWindow(GLFWWindow * gameWindow)
{
	static bool firstTime = true;

	if (firstTime)
	{
		currentWindow = gameWindow;
		currentWindow->Load();
		firstTime = false;
	}	
}

SceneManager& SceneManager::instance()
{
	static SceneManager *instance = new SceneManager();
	return *instance;
}

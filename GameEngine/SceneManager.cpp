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
	unsigned int maxThreads = thread::hardware_concurrency();

	if (maxThreads < 2)
	{
		while (currentWindow->IsRunning())
		{
			Update();
			Render();
		}
	}
	else
	{
		windowRunning = true;

		thread update = thread(Update());

		windowRunning = false;
	}
}

void SceneManager::Update()
{
	static int count = 0;

	cout << "Update: " << count++;
}

void SceneManager::Render()
{
	static int count = 0;
	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	cout << "Render: " << count++;

	currentWindow->LimitFPS(60);
	currentWindow->WindowEvents();
}

void SceneManager::SetScene(iScene * scene)
{
	delete currentScene;
	currentScene = scene;
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

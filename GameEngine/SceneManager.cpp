#include "SceneManager.h"
#include <iostream>


SceneManager::~SceneManager()
{
	if (swap.joinable())
	{
		swap.join();
	}
	delete currentScene;
}

void SceneManager::Run()
{
	while (tempRunning)
	{
		sceneRunning = true;
		unsigned int maxThreads = thread::hardware_concurrency();

		windowRunning = true;

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

			thread update = thread(&SceneManager::ThreadUpdate, this);

			while (currentWindow->IsRunning() && windowRunning)
			{
				Render();
			}

			windowRunning = false;
			update.join();
		}

		sceneRunning = false;

		if (currentWindow->IsRunning())
		{
			FinishSwapScene();
		}
		else
		{
			tempRunning = false;
		}
	}
}

void SceneManager::Update()
{
	currentScene->Update();
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentScene->Render();

	currentWindow->LimitFPS(60);
	currentWindow->WindowEvents();
}

void SceneManager::StartSwapScene(iScene * scene)
{
	tempRunning = windowRunning;
	windowRunning = false;
	newScene = scene;
}

void SceneManager::FinishSwapScene()
{
	if (currentScene != nullptr)
	{
		currentScene->Close();
	}

	delete currentScene;

	currentScene = newScene;
	currentScene->Load();
}

void SceneManager::SetScene(iScene * scene)
{
	StartSwapScene(scene);

	if (currentScene == nullptr)
	{
		tempRunning = true;
		FinishSwapScene();
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

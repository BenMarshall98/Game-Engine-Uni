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
			while (currentWindow->IsRunning() && windowRunning)
			{
				thread update = thread(&SceneManager::Update, this);
				Render();
				update.join();

				currentWindow->LimitFPS(60);
				currentWindow->WindowEvents();
			}
		}

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

void SceneManager::Render()
{	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentScene->Render();
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

	if (loggingManager->HasSevereMessage())
	{
		tempRunning = false;
	}
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

SceneManager * SceneManager::Instance()
{
	if (instance == nullptr)
	{
		instance = new SceneManager();
	}
	return instance;
}

void SceneManager::Resize(int width, int height)
{
	if (currentScene != nullptr)
	{
		currentScene->Resize(width, height);
	}
}

SceneManager * SceneManager::instance = nullptr;
#include "SceneManager.h"
#include "EntityManager.h"
#include "glm/glm.hpp"
#include "ComponentPosition.h"
#include "CameraManager.h"
#include <iostream>


SceneManager::~SceneManager()
{
	if (swap.joinable())
	{
		try {
			swap.join();
		}
		catch (...)
		{

		}
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
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				EntityManager::Instance()->Swap();
				Update();
				Render();
			}
		}
		else
		{
			while (currentWindow->IsRunning() && windowRunning)
			{
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				EntityManager::Instance()->Swap();
				CameraManager::Instance()->Swap();

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

	if (LoggingManager::HasSevereMessage())
	{
		tempRunning = false;
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

	CameraManager::Instance()->Resize(width, height);
}

SceneManager * SceneManager::instance = nullptr;
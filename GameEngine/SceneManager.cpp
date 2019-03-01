#include "SceneManager.h"
#include "EntityManager.h"
#include "glm/glm.hpp"
#include "ComponentPosition.h"
#include "CameraManager.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "LightManager.h"
#include "LoggingManager.h"
#include "PhysicsManager.h"
#include "ScriptingManager.h"
#include "SystemManager.h"
#include "ResourceManager.h"
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

	ResourceManager::ClearResources();
	EntityManager * entityManager = EntityManager::Instance();
	//SystemManager * systemManager = SystemManager::Instance();
	//CameraManager * cameraManager = CameraManager::Instance();
	//AudioManager * audioManager = AudioManager::Instance();
	//InputManager * inputManager = InputManager::Instance();
	//LightManager * lightManager = LightManager::Instance();
	//PhysicsManager * physicsManager = PhysicsManager::Instance();
	//ScriptingManager * scriptingManager = ScriptingManager::Instance();
	

	delete entityManager;
	//delete cameraManager;
	//delete audioManager;
	//delete inputManager;
	//delete lightManager;
	//delete physicsManager;
	//delete scriptingManager;
	//delete systemManager;

	delete newScene;
	delete currentScene;
	delete currentWindow;
}

void SceneManager::Run()
{
	while (tempRunning)
	{
		sceneRunning = true;
		unsigned int maxThreads = std::thread::hardware_concurrency();

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

				std::thread update = std::thread(&SceneManager::Update, this);
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
	newScene = nullptr;
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
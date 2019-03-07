#include "SceneManager.h"
#include "AudioManager.h"
#include "CameraManager.h"
#include "EntityManager.h"
#include "LightManager.h"
#include "PhysicsManager.h"
#include "ScriptingManager.h"
#include "SystemManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "TextRender.h"

SceneManager * SceneManager::instance = nullptr;

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	while (!scenes.empty())
	{
		delete scenes.top();
		scenes.top() = nullptr;
		scenes.pop();
	}

	ResourceManager::ClearResources();
	delete EntityManager::Instance();
	delete AudioManager::Instance();
	delete CameraManager::Instance();
	delete LightManager::Instance();
	delete PhysicsManager::Instance();
	delete ScriptingManager::Instance();
	delete SystemManager::Instance();
	delete InputManager::Instance();
	delete TextRender::Instance();	

	delete window;
}

void SceneManager::Update()
{
	scenes.top()->Update();
}

void SceneManager::Render()
{
	scenes.top()->Render();
}

void SceneManager::Swap()
{
	scenes.top()->Swap();
	EntityManager::Instance()->Swap();
	CameraManager::Instance()->Swap();
}

void SceneManager::Resize(int width, int height)
{
	CameraManager::Instance()->Resize(width, height);
}

void SceneManager::SetWindow(Window * pWindow)
{
	static bool firstTime = true;

	if (firstTime)
	{
		window = pWindow;
		window->Load();
		firstTime = false;
	}
}

void SceneManager::StartNewScene(iScene * scene)
{
	tempRunning = windowRunning;
	windowRunning = false;
	newScene = scene;
	newSceneBool = true;
}

void SceneManager::StartSwapScene(iScene * scene)
{
	tempRunning = windowRunning;
	windowRunning = false;
	newScene = scene;
	swapSceneBool = true;
}

void SceneManager::StartCloseScene(int noOfScene)
{
	tempRunning = windowRunning;
	windowRunning = false;
	scenesToClose = noOfScene;
	closeSceneBool = true;
}

void SceneManager::StartCloseWindow()
{
	windowRunning = false;
	closeWindowBool = true;
}

void SceneManager::FinishNewScene()
{
	scenes.push(newScene);
	scenes.top()->Load();
	newScene = nullptr;

	if (LoggingManager::HasSevereMessage())
	{
		tempRunning = false;
	}

	newSceneBool = false;
}

void SceneManager::FinishSwapScene()
{
	scenes.top()->Close();
	delete scenes.top();
	scenes.pop();

	scenes.push(newScene);
	scenes.top()->Load();
	newScene = nullptr;

	if (LoggingManager::HasSevereMessage())
	{
		tempRunning = false;
	}

	swapSceneBool = false;
}

void SceneManager::FinishCloseScene()
{
	for (int i = 0; i < scenesToClose; i++)
	{
		scenes.top()->Close();
		delete scenes.top();
		scenes.pop();
	}

	if (LoggingManager::HasSevereMessage())
	{
		tempRunning = false;
	}

	closeSceneBool = false;
}

void SceneManager::FinishCloseWindow()
{
	tempRunning = false;
	closeWindowBool = false;
}

void SceneManager::NewScene(iScene * scene)
{
	StartNewScene(scene);

	if (scenes.empty())
	{
		tempRunning = true;
		FinishNewScene();
	}
}

void SceneManager::SwapScene(iScene * scene)
{
	StartSwapScene(scene);
}

void SceneManager::CloseScene(int noOfScenes)
{
	StartCloseScene(noOfScenes);
}

void SceneManager::CloseWindow()
{
	StartCloseWindow();
}

void SceneManager::Run()
{
	while (tempRunning)
	{
		if (scenes.empty())
		{
			break;
		}

		windowRunning = true;

		while (window->IsRunning() && windowRunning)
		{
			Swap();
			std::thread update = std::thread(&SceneManager::Update, this);
			Render();
			update.join();
			window->LimitFPS(60);
			window->WindowEvents();
		}

		if (window->IsRunning())
		{
			if (newSceneBool)
			{
				FinishNewScene();
			}
			else if (swapSceneBool)
			{
				FinishSwapScene();
			}
			else if (closeSceneBool)
			{
				FinishCloseScene();
			}
			else if (closeWindowBool)
			{
				FinishCloseWindow();
			}
		}
		else
		{
			tempRunning = false;
		}
	}
}
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
#include "ConfigLoader.h"
#include "GLFWWindow.h"
#include "RenderManager.h"
#include <iostream>

std::ofstream SceneManager::stream; 
SceneManager * SceneManager::instance = nullptr;
std::string SceneManager::windowName = "GLFW";

SceneManager::SceneManager() : newScene(nullptr), window(nullptr)
{
}


SceneManager::~SceneManager()
{
	ConfigLoader::UpdateEngineConfig();
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

	delete window;
}

//Configures the engine
void SceneManager::ConfigEngine(std::string configFile)
{
	ConfigLoader::LoadEngineConfig(configFile);

	if (windowName == "GLFW")
	{
		window = new GLFWWindow();
		window->Load();
	}

	RenderManager::Instance();
}

//Updates the current scene
void SceneManager::Update()
{
	scenes.top()->Update();
}

//Renders the current scene
void SceneManager::Render()
{
	scenes.top()->Render();
}

//Keeps scene concurrency complient
void SceneManager::Swap()
{
	scenes.top()->Swap();
	EntityManager::Instance()->Swap();
	CameraManager::Instance()->Swap();
	EntityManager::Instance()->Update();
}

//Resizes the window
void SceneManager::Resize(const int width, const int height)
{
	CameraManager::Instance()->Resize(width, height);
}

//Creates new scene
void SceneManager::StartNewScene(iScene * const scene)
{
	tempRunning = windowRunning;
	windowRunning = false;
	newScene = scene;
	newSceneBool = true;
}

//Replaces current scene with another
void SceneManager::StartSwapScene(iScene * const scene)
{
	tempRunning = windowRunning;
	windowRunning = false;
	newScene = scene;
	swapSceneBool = true;
}

//Closes N number of scenes
void SceneManager::StartCloseScene(const int noOfScene)
{
	tempRunning = windowRunning;
	windowRunning = false;
	scenesToClose = noOfScene;
	closeSceneBool = true;
}

//Closes window
void SceneManager::StartCloseWindow()
{
	windowRunning = false;
	closeWindowBool = true;
}

//Finishes create new scene
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

//Finishes replace current scene with another
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

//Finishes close N number of scenes
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

//Finishes close window
void SceneManager::FinishCloseWindow()
{
	tempRunning = false;
	closeWindowBool = false;
}

//Creates new scene
void SceneManager::NewScene(iScene * const scene)
{
	StartNewScene(scene);

	if (scenes.empty())
	{
		tempRunning = true;
		FinishNewScene();
	}
}

//Replaces current scene with another
void SceneManager::SwapScene(iScene * const scene)
{
	StartSwapScene(scene);
}

//Closes N number of scenes
void SceneManager::CloseScene(const int noOfScenes)
{
	StartCloseScene(noOfScenes);
}

//Closes window
void SceneManager::CloseWindow()
{
	StartCloseWindow();
}

//Runs the game engine
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
			if (setScreenSize)
			{
				window->ChangeSize(tempWidth, tempHeight);
				setScreenSize = false;
			}
			if (setFullscreen)
			{
				window->Fullscreen();
				setFullscreen = false;
			}
			std::thread update = std::thread(&SceneManager::Update, this);
			Render();
			update.join();
			Swap();
			window->WindowEvents();
			window->LimitFPS();
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
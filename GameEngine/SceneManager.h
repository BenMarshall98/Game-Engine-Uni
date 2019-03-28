#pragma once

#include <thread>

#include "iScene.h"
#include "Window.h"
#include "LoggingManager.h"
#include <string>
#include <stack>
#include <ostream>
#include <fstream>

class SceneManager
{
private:
	static std::ofstream stream;
	static SceneManager * instance;
	static std::string windowName;

	std::stack<iScene *> scenes;
	iScene * newScene;
	Window * window;

	bool tempRunning = false;
	bool windowRunning = false;
	bool newSceneBool = false;
	bool swapSceneBool = false;
	bool closeSceneBool = false;
	bool closeWindowBool = false;
	bool setScreenSize = false;
	bool setFullscreen = false;
	int tempWidth;
	int tempHeight;

	int scenesToClose = 0;

	SceneManager();
	void Update();
	void Render();
	void Swap();

	void StartNewScene(iScene * scene);
	void FinishNewScene();

	void StartSwapScene(iScene * scene);
	void FinishSwapScene();

	void StartCloseScene(int noOfScene);
	void FinishCloseScene();

	void StartCloseWindow();
	void FinishCloseWindow();

public:
	static inline SceneManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new SceneManager();
		}

		return instance;
	}

	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(SceneManager&) = delete;

	~SceneManager();

	void Run();

	void Resize(int width, int height);

	void NewScene(iScene * scene);
	void SwapScene(iScene * scene);
	void CloseScene(int noOfScenes = 1);
	void CloseWindow();

	void ConfigEngine(std::string configFile);

	static void InitWindow(std::string pWindowName)
	{
		windowName = pWindowName;
	}

	static inline void InitFileStream(std::string pLogFile)
	{
		stream = std::ofstream(pLogFile);
	}

	static inline std::ostream& GetStream()
	{
		if (stream)
		{
			return stream;
		}

		return std::cout;
	}

	inline void ChangeSize(float width, float height)
	{
		setScreenSize = true;
		tempWidth = width;
		tempHeight = height;
	}

	inline void FullScreen()
	{
		setFullscreen = true;
	}
};


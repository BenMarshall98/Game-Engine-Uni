#pragma once
#include "iScene.h"
#include "GLFWWindow.h"

using namespace std;

class SceneManager
{
private:
	iScene * currentScene;
	GLFWWindow * currentWindow;
	static bool windowRunning;

	SceneManager() : currentScene(nullptr), currentWindow(nullptr) {};

	void Update();
	void Render();

public:

	static SceneManager& instance();

	~SceneManager();

	void Run();
	void SetScene(iScene * scene);
	void SetWindow(GLFWWindow * gameWindow);
};


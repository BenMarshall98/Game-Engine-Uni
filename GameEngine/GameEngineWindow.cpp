#include "OpenGL.h"
#include "GameEngineWindow.h"
#include <iostream>

using namespace std;

GameEngineWindow::GameEngineWindow()
{
	/*
	The following code was adapted from the following resource:
		https://learnopengl.com/Getting-started/Hello-Window

	Adaptations include spliting the code across multiple methods,
	and making a methods static (windowResize) so that it will work
	within a class.
	*/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	gameWindow = glfwCreateWindow(800, 600, "Game Engine", nullptr, nullptr);

	if (gameWindow == nullptr)
	{
		//TODO: Change to output to a logging system
		glfwTerminate();
	}

	glfwMakeContextCurrent(gameWindow);
	glfwSetFramebufferSizeCallback(gameWindow, windowResize);
}




GameEngineWindow::~GameEngineWindow()
{
	glfwTerminate();
}

void GameEngineWindow::Run() const
{
	while (!glfwWindowShouldClose(gameWindow))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//TODO: limit the game engine to ~60 fps

		glfwSwapBuffers(gameWindow);
		glfwPollEvents();
	}
}

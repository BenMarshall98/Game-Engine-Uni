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

	gameWindow = glfwCreateWindow(800, 600, "Game Engine", NULL, NULL);

	if (gameWindow == NULL)
	{
		//TODO: Change to output to a logging system
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(gameWindow);
	glfwSetFramebufferSizeCallback(gameWindow, windowResize);
}

void GameEngineWindow::windowResize(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}


GameEngineWindow::~GameEngineWindow()
{
	glfwTerminate();
}

void GameEngineWindow::Run()
{
	while (!glfwWindowShouldClose(gameWindow))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(gameWindow);
		glfwPollEvents();
	}
}

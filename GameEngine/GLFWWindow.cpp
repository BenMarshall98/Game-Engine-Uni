#include "GLFWWindow.h"
#include <Windows.h>
#include <iostream>

using namespace std;

GLFWWindow::GLFWWindow() : gameWindow(nullptr)
{
	
}

GLFWWindow::~GLFWWindow()
{
	glfwTerminate();
}

void GLFWWindow::Load()
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
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
	}
}
//
//void GLFWWindow::Run() const
//{
//	while (!glfwWindowShouldClose(gameWindow))
//	{
//		
//
//		//TODO: limit the game engine to ~60 fps
//
//		glfwSwapBuffers(gameWindow);
//		glfwPollEvents();
//	}
//}

void GLFWWindow::WindowEvents()
{
	glfwSwapBuffers(gameWindow);
	glfwPollEvents();
}

bool GLFWWindow::IsRunning()
{
	return !glfwWindowShouldClose(gameWindow);
}

void GLFWWindow::LimitFPS(float FPS)
{
	double timeLapsed = glfwGetTime();

	double timeLeft = (1 / FPS) - timeLapsed;

	if (timeLeft > 0)
	{
		Sleep(timeLeft * 1000);
	}

	glfwSetTime(0);
}
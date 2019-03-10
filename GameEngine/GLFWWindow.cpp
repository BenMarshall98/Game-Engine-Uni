#include "GLFWWindow.h"
#include "SceneManager.h"
#include "InputManager.h"
#include <Windows.h>
#include <iostream>

int GLFWWindow::height = 600;
int GLFWWindow::width = 800;

GLFWWindow::GLFWWindow() : gameWindow(nullptr)
{
	
}

GLFWWindow::~GLFWWindow()
{
	glfwTerminate();
}

void GLFWWindow::windowResize(GLFWwindow * const window, const int pWidth, const int pHeight)
{
	glViewport(0, 0, pWidth, pHeight);
	height = pHeight;
	width = pWidth;
	SceneManager * const sceneManager = SceneManager::Instance();
	sceneManager->Resize(pWidth, pHeight);
};

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

	InputManager::Instance(new GLFWInput(gameWindow));
}

void GLFWWindow::WindowEvents()
{
	glfwSwapBuffers(gameWindow);
	glfwPollEvents();
}

bool GLFWWindow::IsRunning()
{
	return !glfwWindowShouldClose(gameWindow);
}

void GLFWWindow::LimitFPS(const float FPS)
{
	double timeLapsed = glfwGetTime();

	static double average = 0;
	static int count = 0;

	if (count == 0)
	{
		average += (1.0 / timeLapsed);
		count++;
	}
	else
	{
		average = ((average * count) + (1.0 / timeLapsed)) / (count + 1);
		count++;
	}

	if (count == 60)
	{
		const std::string title = "Game Engine: " + std::to_string((int)average) + "fps";
		glfwSetWindowTitle(gameWindow, title.c_str());
		average = 0;
		count = 0;
	}

	//cout << timeLapsed << endl;

	timeLapsed = glfwGetTime();

	const double timeLeft = (1 / FPS) - timeLapsed;

	if (timeLeft > 0)
	{
		Sleep(timeLeft * 1000);
	}

	glfwSetTime(0);
}
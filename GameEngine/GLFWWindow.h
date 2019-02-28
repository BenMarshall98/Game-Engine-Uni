#pragma once

#include "OpenGL.h"
#include "Window.h"
#include <GLFW/glfw3.h>

//TODO: Make the Window generic
class GLFWWindow : public Window
{
private:
	GLFWwindow * gameWindow;

	static inline void windowResize(GLFWwindow* window, const int pWidth, const int pHeight);

	GLFWWindow(const GLFWWindow & window) = delete;
	GLFWWindow& operator=(const GLFWWindow & window) = delete;

public:
	GLFWWindow();
	~GLFWWindow() override;

	void Load() override;
	void LimitFPS(float FPS) override;
	void WindowEvents() override;
	bool IsRunning() override;
};


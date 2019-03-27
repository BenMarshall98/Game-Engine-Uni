#pragma once

#include "OpenGL.h"
#include "Window.h"
#include <GLFW/glfw3.h>

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
	void LimitFPS() override;
	void WindowEvents() override;
	bool IsRunning() override;

	void ChangeSize(const float width, const float height) override;
	void Fullscreen() override;
};


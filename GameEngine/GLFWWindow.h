#include "glad.h"
#include <GLFW/glfw3.h>

#pragma once
class GLFWWindow
{
	private:
		GLFWwindow * gameWindow;

		static inline void windowResize(GLFWwindow* window, const int pWidth, const int pHeight)
		{
			glViewport(0, 0, pWidth, pHeight);
			width = pWidth;
			height = pHeight;
		};

		GLFWWindow(const GLFWWindow & window) = delete;
		GLFWWindow& operator=(const GLFWWindow & window) = delete;

	public:
		GLFWWindow();
		~GLFWWindow();

		void Load();
		void LimitFPS(float FPS);
		void WindowEvents();
		bool IsRunning();

		static int width;
		static int height;
};


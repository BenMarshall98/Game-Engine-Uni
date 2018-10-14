#include <GLFW/glfw3.h>

#pragma once
class GLFWWindow
{
	private:
		GLFWwindow * gameWindow;

		static inline void windowResize(GLFWwindow* window, const int width, const int height)
		{
			glViewport(0, 0, width, height);
		};

		GLFWWindow(const GLFWWindow & window) = delete;
		GLFWWindow& operator=(const GLFWWindow & window) = delete;

	public:
		GLFWWindow();
		~GLFWWindow();

		void Run() const;
		void Load();
};


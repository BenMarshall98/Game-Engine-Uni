#include <GLFW/glfw3.h>

#pragma once
class GameEngineWindow
{
	private:
		GLFWwindow * gameWindow;

		static void windowResize(GLFWwindow* window, int width, int height);

	public:
		GameEngineWindow();
		~GameEngineWindow();

		void Run();
};


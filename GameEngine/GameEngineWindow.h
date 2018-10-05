#include <GLFW/glfw3.h>

#pragma once
class GameEngineWindow
{
	private:
		GLFWwindow * gameWindow;

		static inline void windowResize(GLFWwindow* window, const int width, const int height)
		{
			glViewport(0, 0, width, height);
		};
		GameEngineWindow(const GameEngineWindow & window) = delete;
		GameEngineWindow& operator=(const GameEngineWindow & window) = delete;

	public:
		GameEngineWindow();
		~GameEngineWindow();

		void Run() const;
		void Load();
};


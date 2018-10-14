#include "GLFWWindow.h"
#include "SceneManager.h"

int main(void)
{
	SceneManager& sceneManger = SceneManager::instance();

	GLFWWindow entryPoint;

	//Load Game Code into the Game Engine here
	entryPoint.Load();
	entryPoint.Run();

	return 0;
}
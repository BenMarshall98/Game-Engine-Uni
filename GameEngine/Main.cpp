#include "GameEngineWindow.h"

int main(void)
{
	GameEngineWindow entryPoint;

	//Load Game Code into the Game Engine here
	entryPoint.Load();
	entryPoint.Run();

	return 0;
}
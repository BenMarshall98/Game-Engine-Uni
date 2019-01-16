#include "InputManager.h"

#define I(s) #s
vector<string> InputConverter::GameInputName = { GAMEINPUT(I) };
#undef I

map<string, int> InputConverter::GameInputMappings = map<string, int>();

void InputConverter::FirstTimeMapSetUp()
{
	for (int i = 0; i < GameInputName.size(); i++)
	{
		istringstream stream(GameInputName[i]);

		string inputName;
		string device;
		int code;
		string dummy;

		stream >> inputName >> dummy >> device >> dummy >> dummy >> dummy >> code >> dummy >> dummy;

		DeviceType deviceType;

		if (device == "GAMEPAD")
		{
			deviceType = GAMEPAD;
		}
		else if (device == "KEYBOARD")
		{
			deviceType = KEYBOARD;
		}
		else
		{
			deviceType = MOUSE;
		}

		int inputCode = (deviceType << DEVICE | code << CODE);

		GameInputMappings.insert(pair<string, int>(inputName, inputCode));
	}
}

GameInput InputConverter::StringToEnum(string input)
{
	static bool firstTime = true;

	if (firstTime)
	{
		FirstTimeMapSetUp();
		firstTime = false;
	}

	map<string, int>::iterator it = GameInputMappings.find(input);

	if (it != GameInputMappings.end())
	{
		return GameInput(it->second);
	}
}
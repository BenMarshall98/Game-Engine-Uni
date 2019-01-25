#pragma once

#include <string>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

//http://www.cplusplus.com/forum/beginner/174993/


enum class DeviceType
{
	KEYBOARD = 1,
	MOUSE    = 2,
	GAMEPAD  = 3
};

enum class Offset
{
	DEVICE   = 8,
	CODE     = 16
};

#define GAMEINPUT(I) \
	I(KEYBOARD_A = DeviceType::KEYBOARD << Offset::DEVICE | 65 << Offset::CODE), \
	I(KEYBOARD_B = DeviceType::KEYBOARD << Offset::DEVICE | 66 << Offset::CODE), \
	I(KEYBOARD_C = DeviceType::KEYBOARD << Offset::DEVICE | 67 << Offset::CODE), \
	I(KEYBOARD_D = DeviceType::KEYBOARD << Offset::DEVICE | 68 << Offset::CODE), \
	I(KEYBOARD_E = DeviceType::KEYBOARD << Offset::DEVICE | 69 << Offset::CODE), \
	I(KEYBOARD_F = DeviceType::KEYBOARD << Offset::DEVICE | 70 << Offset::CODE), \
	I(KEYBOARD_G = DeviceType::KEYBOARD << Offset::DEVICE | 71 << Offset::CODE), \
	I(KEYBOARD_H = DeviceType::KEYBOARD << Offset::DEVICE | 72 << Offset::CODE), \
	I(KEYBOARD_I = DeviceType::KEYBOARD << Offset::DEVICE | 73 << Offset::CODE), \
	I(KEYBOARD_J = DeviceType::KEYBOARD << Offset::DEVICE | 74 << Offset::CODE), \
	I(KEYBOARD_K = DeviceType::KEYBOARD << Offset::DEVICE | 75 << Offset::CODE), \
	I(KEYBOARD_L = DeviceType::KEYBOARD << Offset::DEVICE | 76 << Offset::CODE), \
	I(KEYBOARD_M = DeviceType::KEYBOARD << Offset::DEVICE | 77 << Offset::CODE), \
	I(KEYBOARD_N = DeviceType::KEYBOARD << Offset::DEVICE | 78 << Offset::CODE), \
	I(KEYBOARD_O = DeviceType::KEYBOARD << Offset::DEVICE | 79 << Offset::CODE), \
	I(KEYBOARD_P = DeviceType::KEYBOARD << Offset::DEVICE | 80 << Offset::CODE), \
	I(KEYBOARD_Q = DeviceType::KEYBOARD << Offset::DEVICE | 81 << Offset::CODE), \
	I(KEYBOARD_R = DeviceType::KEYBOARD << Offset::DEVICE | 82 << Offset::CODE), \
	I(KEYBOARD_S = DeviceType::KEYBOARD << Offset::DEVICE | 83 << Offset::CODE), \
	I(KEYBOARD_T = DeviceType::KEYBOARD << Offset::DEVICE | 84 << Offset::CODE), \
	I(KEYBOARD_U = DeviceType::KEYBOARD << Offset::DEVICE | 85 << Offset::CODE), \
	I(KEYBOARD_V = DeviceType::KEYBOARD << Offset::DEVICE | 86 << Offset::CODE), \
	I(KEYBOARD_W = DeviceType::KEYBOARD << Offset::DEVICE | 87 << Offset::CODE), \
	I(KEYBOARD_X = DeviceType::KEYBOARD << Offset::DEVICE | 88 << Offset::CODE), \
	I(KEYBOARD_Y = DeviceType::KEYBOARD << Offset::DEVICE | 89 << Offset::CODE), \
	I(KEYBOARD_Z = DeviceType::KEYBOARD << Offset::DEVICE | 90 << Offset::CODE), \
													 \
	I(KEYBOARD_0 = DeviceType::KEYBOARD << Offset::DEVICE | 48 << Offset::CODE), \
	I(KEYBOARD_1 = DeviceType::KEYBOARD << Offset::DEVICE | 49 << Offset::CODE), \
	I(KEYBOARD_2 = DeviceType::KEYBOARD << Offset::DEVICE | 50 << Offset::CODE), \
	I(KEYBOARD_3 = DeviceType::KEYBOARD << Offset::DEVICE | 51 << Offset::CODE), \
	I(KEYBOARD_4 = DeviceType::KEYBOARD << Offset::DEVICE | 52 << Offset::CODE), \
	I(KEYBOARD_5 = DeviceType::KEYBOARD << Offset::DEVICE | 53 << Offset::CODE), \
	I(KEYBOARD_6 = DeviceType::KEYBOARD << Offset::DEVICE | 54 << Offset::CODE), \
	I(KEYBOARD_7 = DeviceType::KEYBOARD << Offset::DEVICE | 55 << Offset::CODE), \
	I(KEYBOARD_8 = DeviceType::KEYBOARD << Offset::DEVICE | 56 << Offset::CODE), \
	I(KEYBOARD_9 = DeviceType::KEYBOARD << Offset::DEVICE | 57 << Offset::CODE), \
	\
	I(KEYBOARD_UP = DeviceType::KEYBOARD << Offset::DEVICE | 265 << Offset::CODE), \
	I(KEYBOARD_DOWN = DeviceType::KEYBOARD << Offset::DEVICE | 264 << Offset::CODE), \
	I(KEYBOARD_LEFT = DeviceType::KEYBOARD << Offset::DEVICE | 263 << Offset::CODE), \
	I(KEYBOARD_RIGHT = DeviceType::KEYBOARD << Offset::DEVICE | 262 << Offset::CODE), \
	I(KEYBOARD_SPACE = DeviceType::KEYBOARD << Offset::DEVICE | 32 << Offset::CODE), \
	I(KEYBOARD_SHIFT = DeviceType::KEYBOARD << Offset::DEVICE | 240 << Offset::CODE), \
	\
	I(MOUSE_UP = DeviceType::MOUSE << Offset::DEVICE | 0 << Offset::CODE), \
	I(MOUSE_DOWN = DeviceType::MOUSE << Offset::DEVICE | 1 << Offset::CODE), \
	I(MOUSE_LEFT = DeviceType::MOUSE << Offset::DEVICE | 2 << Offset::CODE), \
	I(MOUSE_RIGHT = DeviceType::MOUSE << Offset::DEVICE | 3 << Offset::CODE), \
	I(MOUSE_LEFT_PRESS = DeviceType::MOUSE << Offset::DEVICE | 4 << Offset::CODE), \
	I(MOUSE_RIGHT_PRESS = DeviceType::MOUSE << Offset::DEVICE | 5 << Offset::CODE), \
	\
	I(GAMEPAD_A = DeviceType::GAMEPAD << Offset::DEVICE | 0 << Offset::CODE), \
	I(GAMEPAD_B = DeviceType::GAMEPAD << Offset::DEVICE | 1 << Offset::CODE), \
	I(GAMEPAD_X = DeviceType::GAMEPAD << Offset::DEVICE | 2 << Offset::CODE), \
	I(GAMEPAD_Y = DeviceType::GAMEPAD << Offset::DEVICE | 3 << Offset::CODE), \
	I(GAMEPAD_LB = DeviceType::GAMEPAD << Offset::DEVICE | 4 << Offset::CODE), \
	I(GAMEPAD_RB = DeviceType::GAMEPAD << Offset::DEVICE | 5 << Offset::CODE), \
	I(GAMEPAD_SELECT = DeviceType::GAMEPAD << Offset::DEVICE | 6 << Offset::CODE), \
	I(GAMEPAD_START = DeviceType::GAMEPAD << Offset::DEVICE | 7 << Offset::CODE), \
	I(GAMEPAD_L = DeviceType::GAMEPAD << Offset::DEVICE | 8 << Offset::CODE), \
	I(GAMEPAD_R = DeviceType::GAMEPAD << Offset::DEVICE | 9 << Offset::CODE), \
	I(GAMEPAD_UP = DeviceType::GAMEPAD << Offset::DEVICE | 10 << Offset::CODE), \
	I(GAMEPAD_RIGHT = DeviceType::GAMEPAD << Offset::DEVICE | 11 << Offset::CODE), \
	I(GAMEPAD_DOWN = DeviceType::GAMEPAD << Offset::DEVICE | 12 << Offset::CODE), \
	I(GAMEPAD_LEFT = DeviceType::GAMEPAD << Offset::DEVICE | 13 << Offset::CODE), \
	I(GAMEPAD_L_LEFT = DeviceType::GAMEPAD << Offset::DEVICE | 14 << Offset::CODE), \
	I(GAMEPAD_L_RIGHT = DeviceType::GAMEPAD << Offset::DEVICE | 15 << Offset::CODE), \
	I(GAMEPAD_L_DOWN = DeviceType::GAMEPAD << Offset::DEVICE | 16 << Offset::CODE), \
	I(GAMEPAD_L_UP = DeviceType::GAMEPAD << Offset::DEVICE | 17 << Offset::CODE), \
	I(GAMEPAD_LT = DeviceType::GAMEPAD << Offset::DEVICE | 23 << Offset::CODE), \
	I(GAMEPAD_RT = DeviceType::GAMEPAD << Offset::DEVICE | 25 << Offset::CODE), \
	I(GAMEPAD_R_DOWN = DeviceType::GAMEPAD << Offset::DEVICE | 20 << Offset::CODE), \
	I(GAMEPAD_R_UP = DeviceType::GAMEPAD << Offset::DEVICE | 21 << Offset::CODE), \
	I(GAMEPAD_R_LEFT = DeviceType::GAMEPAD << Offset::DEVICE | 18 << Offset::CODE), \
	I(GAMEPAD_R_RIGHT = DeviceType::GAMEPAD << Offset::DEVICE | 19 << Offset::CODE)

#define I(e) e
static enum class GameInput { GAMEINPUT(I) };
#undef I

class InputConverter
{
private:
	InputConverter();
#define I(s) #s
	static vector<string> GameInputName;
#undef I

	static void FirstTimeMapSetUp();
	static map<string, int> GameInputMappings;

public:
	static GameInput StringToEnum(string & input);
};
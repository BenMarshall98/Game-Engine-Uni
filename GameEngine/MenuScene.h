#pragma once

#include "iScene.h"
#include <vector>
#include "MenuItem.h"
#include "glm/glm.hpp"
#include "InputManager.h"

class MenuScene : public iScene
{
private:
	std::vector<MenuItem *> menuItems;

	std::vector<GameInput> upMenu = {
		GameInput::KEYBOARD_W,
		GameInput::KEYBOARD_UP,
		GameInput::GAMEPAD_UP,
		GameInput::GAMEPAD_L_UP
	};
	std::vector<GameInput> downMenu = {
		GameInput::KEYBOARD_S,
		GameInput::KEYBOARD_DOWN,
		GameInput::GAMEPAD_DOWN,
		GameInput::GAMEPAD_L_DOWN
	};

	std::vector<GameInput> selectMenu = {
		GameInput::KEYBOARD_ENTER,
		GameInput::GAMEPAD_A
	};

	glm::vec3 backgroundColour = glm::vec3(0);
	int currentSelected = 0;
	int totalSelectable = 0;

public:
	explicit MenuScene(const std::string & pFileName);
	~MenuScene();

	MenuScene& operator=(const MenuScene&) = delete;
	MenuScene(MenuScene&) = delete;

	void Update() override;
	void Render() override;
	void Swap() override;

	void Load() override;
	void Close() override;

};


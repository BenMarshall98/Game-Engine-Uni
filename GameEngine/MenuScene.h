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
		GameInput::KEYBOARD_UP
	};
	std::vector<GameInput> downMenu = {
		GameInput::KEYBOARD_S,
		GameInput::KEYBOARD_DOWN
	};

	std::vector<GameInput> selectMenu = {
		GameInput::KEYBOARD_ENTER
	};

	glm::vec3 backgroundColour = glm::vec3(0);
	int currentSelected = 0;
	int totalSelectable = 0;

public:
	MenuScene(std::string pFileName);
	~MenuScene();

	void Update() override;
	void Render() override;
	void Swap() override;

	void Load() override;
	void Close() override;

};


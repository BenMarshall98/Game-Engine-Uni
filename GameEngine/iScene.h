#pragma once

#include <string>
#include <vector>
#include "InputMapping.h"

class iScene
{
protected:
	std::string fileName;

	std::vector<GameInput> escapeMenu = {
		GameInput::KEYBOARD_ESCAPE,
		GameInput::GAMEPAD_START
	};

public:
	explicit iScene(const std::string & pFileName);
	virtual ~iScene();

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Swap() = 0;

	virtual void Load() = 0;
	virtual void Close() = 0;
};


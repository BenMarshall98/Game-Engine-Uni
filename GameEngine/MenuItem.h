#pragma once

#include "glm/glm.hpp"

#include <string>

enum class MenuType
{
	Text,
	Button
};

class MenuItem
{
private:
	MenuType menuType;

protected:
	std::string text;
	std::string align;
	float fontSize;
	glm::vec3 fontColour;
	glm::vec2 location;

public:
	MenuItem(MenuType pMenuType, std::string pText, std::string pAlign, float pFontSize, glm::vec3 pFontColour, glm::vec2 pLocation);

	virtual ~MenuItem();

	virtual void Update(int height, int width) = 0;
	virtual void Render() = 0;

	inline MenuType GetMenuType()
	{
		return menuType;
	}

	virtual void Select() = 0;
	virtual void Highlight() = 0;
};


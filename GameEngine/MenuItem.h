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
	MenuItem(MenuType pMenuType, const std::string & pText, const std::string & pAlign, float pFontSize, const glm::vec3 & pFontColour, const glm::vec2 & pLocation);

	virtual ~MenuItem();

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Swap() = 0;

	inline MenuType GetMenuType()
	{
		return menuType;
	}

	virtual void Select() = 0;
	virtual void Highlight() = 0;
	virtual void UnHighlight() = 0;
};


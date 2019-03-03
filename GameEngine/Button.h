#pragma once

#include "MenuItem.h"

class Button : public MenuItem
{
private:
	glm::vec3 highlighted;
	std::string function;

public:
	Button(std::string pText, std::string pAlign, float pFontSize, glm::vec3 pFontColour, glm::vec3 pHighlighted, glm::vec2 pLocation, std::string pFunction);
	~Button();

	void Update(int height, int widht) override;
	void Render() override;
	void Select() override;
	void Highlight() override;
};


#pragma once

#include "MenuItem.h"
#include "TextRender.h"

class Button : public MenuItem
{
private:
	glm::vec3 highlightColour;
	std::string function;
	bool updateHighlighted = false;
	bool renderHighlighted = false;
	PixelLocation updateLocation;
	PixelLocation renderLocation;


public:
	Button(std::string pText, std::string pAlign, float pFontSize, glm::vec3 pFontColour, glm::vec3 pHighlighted, glm::vec2 pLocation, std::string pFunction);
	~Button();

	void Update() override;
	void Render() override;
	void Swap() override;
	void Select() override;
	void Highlight() override;
	void UnHighlight() override;
};


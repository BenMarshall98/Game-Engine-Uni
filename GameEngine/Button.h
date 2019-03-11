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
	Button(const std::string & pText, const std::string & pAlign, float pFontSize, const glm::vec3 & pFontColour, const glm::vec3 & pHighlighted, const glm::vec2 & pLocation, const std::string & pFunction);
	~Button();

	void Update() override;
	void Render() override;
	void Swap() override;
	void Select() override;
	void Highlight() override;
	void UnHighlight() override;
};


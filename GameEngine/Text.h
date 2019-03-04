#pragma once

#include "MenuItem.h"
#include "TextRender.h"

class Text : public MenuItem
{
private:
	PixelLocation updateLocation;
	PixelLocation renderLocation;

public:
	Text(std::string pText, std::string pAlign, float pFontSize, glm::vec3 pFontColour, glm::vec2 pLocation);
	~Text();

	void Update() override;
	void Render() override;
	void Swap() override;
	void Select() override {}
	void Highlight() override {}
	void UnHighlight() override {}
};


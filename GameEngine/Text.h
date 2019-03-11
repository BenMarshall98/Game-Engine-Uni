#pragma once

#include "MenuItem.h"
#include "TextRender.h"

class Text : public MenuItem
{
private:
	PixelLocation updateLocation;
	PixelLocation renderLocation;

public:
	Text(const std::string & pText, const std::string & pAlign, float pFontSize, const glm::vec3 & pFontColour, const glm::vec2 & pLocation);
	~Text();

	void Update() override;
	void Render() override;
	void Swap() override;
	void Select() override;
	void Highlight() override;
	void UnHighlight() override;
};


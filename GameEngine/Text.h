#pragma once

#include "MenuItem.h"

class Text : public MenuItem
{
public:
	Text(std::string pText, std::string pAlign, float pFontSize, glm::vec3 pFontColour, glm::vec2 pLocation);
	~Text();

	void Update(int height, int widht) override;
	void Render() override;
	void Select() override {}
	void Highlight() override {}
};


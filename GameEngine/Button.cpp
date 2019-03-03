#include "Button.h"



Button::Button(std::string pText, std::string pAlign, float pFontSize, glm::vec3 pFontColour, glm::vec3 pHighlighted, glm::vec2 pLocation, std::string pFunction) :
	MenuItem(MenuType::Button, pText, pAlign, pFontSize, pFontColour, pLocation), highlighted(pHighlighted), function(pFunction)
{
}


Button::~Button()
{
}

void Button::Update(int height, int width)
{

}

void Button::Render()
{

}

void Button::Select()
{

}

void Button::Highlight()
{

}
#include "Text.h"



Text::Text(std::string pText, std::string pAlign, float pFontSize, glm::vec3 pFontColour, glm::vec2 pLocation) : MenuItem(MenuType::Text, pText, pAlign, pFontSize, pFontColour, pLocation)
{
}


Text::~Text()
{
}


void Text::Update(int height, int width)
{

}

void Text::Render()
{

}

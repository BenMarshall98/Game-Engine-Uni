#include "MenuItem.h"
#include "TextRender.h"

MenuItem::MenuItem(MenuType pMenuType, std::string pText, std::string pAlign, float pFontSize, glm::vec3 pFontColour, glm::vec2 pLocation) :
	menuType(pMenuType), text(pText), align(pAlign), fontSize(pFontSize), fontColour(pFontColour)
{
	TextRender::Instance();
}

MenuItem::~MenuItem()
{
}

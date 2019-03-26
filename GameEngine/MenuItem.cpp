#include "MenuItem.h"

MenuItem::MenuItem(const MenuType pMenuType, const std::string & pText, const std::string & pAlign, const float pFontSize, const glm::vec3 & pFontColour, const glm::vec2 & pLocation) :
	menuType(pMenuType), text(pText), align(pAlign), fontSize(pFontSize), fontColour(pFontColour), location(pLocation)
{
}

MenuItem::~MenuItem()
{
}

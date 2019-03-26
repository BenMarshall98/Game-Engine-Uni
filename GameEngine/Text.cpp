#include "Text.h"
#include "RenderManager.h"

Text::Text(const std::string & pText, const std::string & pAlign, const float pFontSize, const glm::vec3 & pFontColour, const glm::vec2 & pLocation) : MenuItem(MenuType::Text, pText, pAlign, pFontSize, pFontColour, pLocation)
{
}


Text::~Text()
{
}


void Text::Update()
{
	updateLocation = RenderManager::Instance()->CalculateTextSize(text, location, fontSize, align);
}

void Text::Render()
{
	RenderManager::Instance()->RenderText(text, renderLocation, fontColour);
}

void Text::Swap()
{
	renderLocation = updateLocation;
}

void Text::Select()
{
}

void Text::Highlight()
{
}

void Text::UnHighlight()
{
}

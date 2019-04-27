#include "Text.h"
#include "RenderManager.h"

Text::Text(const std::string & pText, const std::string & pAlign, const float pFontSize, const glm::vec3 & pFontColour, const glm::vec2 & pLocation) : MenuItem(MenuType::Text, pText, pAlign, pFontSize, pFontColour, pLocation)
{
}


Text::~Text()
{
}

//Updates the text
void Text::Update()
{
	updateLocation = RenderManager::Instance()->CalculateTextSize(text, location, fontSize, align);
}

//Renders the text
void Text::Render()
{
	RenderManager::Instance()->RenderText(text, renderLocation, fontColour);
}

//Keeps the text concurrency complient
void Text::Swap()
{
	renderLocation = updateLocation;
}

//Does nothing
void Text::Select()
{
}

//Does nothing
void Text::Highlight()
{
}

//Does nothing
void Text::UnHighlight()
{
}

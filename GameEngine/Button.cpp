#include "Button.h"


Button::Button(std::string pText, std::string pAlign, float pFontSize, glm::vec3 pFontColour, glm::vec3 pHighlighted, glm::vec2 pLocation, std::string pFunction) :
	MenuItem(MenuType::Button, pText, pAlign, pFontSize, pFontColour, pLocation), highlightColour(pHighlighted), function(pFunction)
{
}


Button::~Button()
{
}

void Button::Update()
{
	updateLocation = TextRender::Instance()->CalculateSize(text, location, fontSize, align);
}

void Button::Render()
{
	if (renderHighlighted)
	{
		TextRender::Instance()->RenderText(text, renderLocation, highlightColour);
	}
	else
	{
		TextRender::Instance()->RenderText(text, renderLocation, fontColour);
	}
}

void Button::Select()
{

}

void Button::Highlight()
{
	updateHighlighted = true;
}

void Button::UnHighlight()
{
	updateHighlighted = false;
}

void Button::Swap()
{
	renderHighlighted = updateHighlighted;
	renderLocation = updateLocation;
}
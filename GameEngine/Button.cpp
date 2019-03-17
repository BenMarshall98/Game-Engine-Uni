#include "Button.h"

#include "ScriptingManager.h"

Button::Button(const std::string & pText, const std::string & pAlign, const float pFontSize, const glm::vec3 & pFontColour, const glm::vec3 & pHighlighted, const glm::vec2 & pLocation, const std::string & pFunction) :
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
	ScriptingManager::Instance()->RunScriptFromScene(function);
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
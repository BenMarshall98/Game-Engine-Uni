#include "TextRender.h"

#include "Window.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

TextRender::TextRender()
{

}

//Calculates the pizel size of the text
PixelLocation TextRender::CalculateSize(const std::string & text, const glm::vec2 & location, const float size, const std::string & align)
{
	const float scale = ((size / 100.0) * (float)Window::GetHeight()) / pixelHeight;

	float x = 0;
	const float y = 0;

	const float minX = 0;
	float minY = 0;
	float maxX = 0;
	float maxY = 0;

	std::string::const_iterator c;

	for (c = text.begin(); c != text.end(); c++)
	{
		const Character * ch = Characters[*c];

		const float xpos = x + ch->Bearing.x * scale;
		const float ypos = y - (ch->Size.y - ch->Bearing.y) * scale;

		const float w = ch->Size.x * scale;
		const float h = ch->Size.y * scale;

		if (ypos < minY)
		{
			minY = ypos;
		}

		if ((ypos + h) > maxY)
		{
			maxY = (ypos + h);
		}

		if ((xpos + w) > maxX)
		{
			maxX = (xpos + w);
		}

		x += (ch->Advance >> 6) * scale;
	}

	const float sizeX = maxX - minX;
	const float sizeY = maxY - minX;
	const float offsetY = 0 - minY;

	const float locX = ((location.x / 100) * Window::GetWidth());
	const float locY = ((1 - (location.y / 100)) * Window::GetHeight());

	if (align == "Centre")
	{
		const float xLoc = locX - (sizeX / 2);
		const float yLoc = locY - (sizeY / 2) + offsetY;
		const PixelLocation pixelLocation = {
			glm::vec2(
				xLoc,
				yLoc
			),
			scale
		};
		return pixelLocation;
	}
	else if (align == "TopLeft")
	{
		const float xLoc = locX;
		const float yLoc = locY;
		const PixelLocation pixelLocation = {
			glm::vec2(
				xLoc,
				yLoc
			),
			scale
		};
		return pixelLocation;
	}
	else if (align == "TopMiddle")
	{
		const float xLoc = locX - (sizeX / 2);
		const float yLoc = locY;
		const PixelLocation pixelLocation = {
			glm::vec2(
				xLoc,
				yLoc
			),
			scale
		};
		return pixelLocation;
	}
	else if (align == "TopRight")
	{
		const float xLoc = locX - sizeX;
		const float yLoc = locY;
		const PixelLocation pixelLocation = {
			glm::vec2(
				xLoc,
				yLoc
			),
			scale
		};
		return pixelLocation;
	}
	else if (align == "MiddleLeft")
	{
		const float xLoc = locX;
		const float yLoc = locY - (sizeY / 2) + offsetY;
		const PixelLocation pixelLocation = {
			glm::vec2(
				xLoc,
				yLoc
			),
			scale
		};
		return pixelLocation;
	}
	else if (align == "MiddleRight")
	{
		const float xLoc = locX - sizeX;
		const float yLoc = locY - (sizeY / 2) + offsetY;
		const PixelLocation pixelLocation = {
			glm::vec2(
				xLoc,
				yLoc
			),
			scale
		};
		return pixelLocation;
	}
	else if (align == "BottomLeft")
	{
		const float xLoc = locX;
		const float yLoc = locY - sizeY + offsetY;
		const PixelLocation pixelLocation = {
			glm::vec2(
				xLoc,
				yLoc
			),
			scale
		};
		return pixelLocation;
	}
	else if (align == "BottomMiddle")
	{
		const float xLoc = locX - (sizeX / 2);
		const float yLoc = locY - sizeY + offsetY;
		const PixelLocation pixelLocation = {
			glm::vec2(
				xLoc,
				yLoc
			),
			scale
		};
		return pixelLocation;
	}
	else if (align == "BottomRight")
	{
		const float xLoc = locX - sizeX;
		const float yLoc = locY - sizeY + offsetY;
		const PixelLocation pixelLocation = {
			glm::vec2(
				xLoc,
				yLoc
			),
			scale
		};
		return pixelLocation;
	}

	return PixelLocation();
}

TextRender::~TextRender()
{
}

#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include "ResourceManager.h"
#include <string>
#include <map>

struct Character {
	public:
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		unsigned int Advance;

		virtual void RenderCharacter() const = 0;
};

struct PixelLocation {
	glm::vec2 location;
	float scale;
};



class TextRender
{
protected:
	Shader * shader;
	const float pixelHeight = 150;
	std::map<char, Character *> Characters = std::map<char, Character *>();

public:
	TextRender();
	virtual void RenderText(const std::string & text, const PixelLocation & pPixelLocation, const glm::vec3 & colour) = 0;
	PixelLocation CalculateSize(const std::string & text, const glm::vec2 & location, float size, const std::string & align);

	~TextRender();

	TextRender& operator=(const TextRender&) = delete;
	TextRender(TextRender&) = delete;
};


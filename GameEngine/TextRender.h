#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include "ResourceManager.h"
#include <string>
#include <map>

struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

struct PixelLocation {
	glm::vec2 location;
	float scale;
};

class TextRender
{
private:
	TextRender();

	Shader * shader;
	unsigned int VAO;
	unsigned int VBO;
	const float pixelHeight = 150;

	static TextRender * instance;

	std::map<char, Character> Characters;

public:
	inline static TextRender * Instance()
	{
		if (instance == nullptr)
		{
			instance = new TextRender();
		}

		return instance;
	}

	void RenderText(std::string text, PixelLocation pPixelLocation, glm::vec3 colour);
	PixelLocation CalculateSize(std::string text, glm::vec2 location, float size, std::string align);

	~TextRender();
};


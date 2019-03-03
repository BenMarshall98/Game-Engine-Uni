#include "TextRender.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "LoggingManager.h"
#include "OpenGL.h"

TextRender * TextRender::instance = nullptr;

TextRender::TextRender()
{
	ResourceManager::LoadShader("TextShader", "TextVertex.vert", "TextFragment.frag");
	shader = ResourceManager::GetShader("TextShader");

	FT_Library ft;

	if (FT_Init_FreeType(&ft))
	{
		LoggingManager::LogMessage(MESSAGE_TYPE::SEVERE, "Error: Could not initialise FreeType Library");
	}

	FT_Face face;

	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
	{
		LoggingManager::LogMessage(MESSAGE_TYPE::SEVERE, "Error: Failed to load font");
	}

	FT_Set_Pixel_Sizes(face, 0, 150);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			LoggingManager::LogMessage(MESSAGE_TYPE::SEVERE, "Error: Failed to load Glyph");
			continue;
		}

		unsigned int texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		Characters.insert(std::pair<char, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRender::RenderText(std::string text, PixelLocation pPixelLocation, glm::vec3 colour)
{

}

PixelLocation TextRender::CalculateSize(std::string text, glm::vec2 location, float size, std::string align)
{

}

TextRender::~TextRender()
{
}

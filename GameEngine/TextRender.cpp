#include "TextRender.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "LoggingManager.h"
#include "OpenGL.h"
#include "Window.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "RenderManager.h"

TextRender * TextRender::instance = nullptr;

TextRender::TextRender() : VAO(0), VBO(0)
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

	FT_Set_Pixel_Sizes(face, 0, pixelHeight);

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

void TextRender::RenderText(const std::string & text, const PixelLocation & pPixelLocation, const glm::vec3 & colour)
{

	shader = ResourceManager::GetShader("TextShader");

	if (!shader)
	{
		ResourceManager::LoadShader("TextShader", "TextVertex.vert", "TextFragment.frag");
		shader = ResourceManager::GetShader("TextShader");
	}

	glViewport(0, 0, Window::GetWidth(), Window::GetHeight());

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 projection = glm::ortho(0.0f, (float)Window::GetWidth(), 0.0f, (float)Window::GetHeight());

	shader->UseShader();
	
	RenderManager::Instance()->SetUniformMatrix4fv(shader, "projection", projection, false);
	RenderManager::Instance()->SetUniform3fv(shader, "textColor", colour);
	RenderManager::Instance()->UseTexture(shader, "text", nullptr, 0);

	glBindVertexArray(VAO);

	float x = pPixelLocation.location.x;
	const float y = pPixelLocation.location.y;

	std::string::const_iterator c;

	for (c = text.begin(); c != text.end(); c++)
	{
		const Character ch = Characters[*c];

		const float xpos = x + ch.Bearing.x * pPixelLocation.scale;
		const float ypos = y - (ch.Size.y - ch.Bearing.y) * pPixelLocation.scale;

		const float w = ch.Size.x * pPixelLocation.scale;
		const float h = ch.Size.y * pPixelLocation.scale;

		float vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0},
			{ xpos, ypos, 0.0, 1.0},
			{ xpos + w, ypos, 1.0, 1.0},

			{ xpos, ypos + h, 0.0, 0.0},
			{ xpos + w, ypos, 1.0, 1.0},
			{ xpos + w, ypos + h, 1.0, 0.0}
		};

		
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * pPixelLocation.scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

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
		const Character ch = Characters[*c];

		const float xpos = x + ch.Bearing.x * scale;
		const float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		const float w = ch.Size.x * scale;
		const float h = ch.Size.y * scale;

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

		x += (ch.Advance >> 6) * scale;
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

	return PixelLocation();
}

TextRender::~TextRender()
{
}

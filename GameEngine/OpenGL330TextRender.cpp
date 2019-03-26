#include "OpenGL330TextRender.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include "OpenGL.h"
#include "RenderManager.h"
#include "LoggingManager.h"
#include "OpenGL330Texture.h"
#include "Window.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

OpenGL330TextRender::OpenGL330TextRender() : VAO(0), VBO(0)
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

		OpenGL330Character * character = new OpenGL330Character();
		character->TextureID = texture;
		character->Size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		character->Bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		character->Advance = face->glyph->advance.x;

		Characters.insert(std::pair<char, Character *>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	RenderManager::Instance()->BindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	RenderManager::Instance()->BindVertexArray(0);
}


OpenGL330TextRender::~OpenGL330TextRender()
{
}

void OpenGL330TextRender::RenderText(const std::string & text, const PixelLocation & pPixelLocation, const glm::vec3 & colour)
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

	const glm::mat4 projection = glm::ortho(0.0f, (float)Window::GetWidth(), 0.0f, (float)Window::GetHeight());

	shader->UseShader();

	UniformLocation * projectionLocation = RenderManager::Instance()->GetUniformLocation(shader, "projection");
	RenderManager::Instance()->SetUniformMatrix4fv(projectionLocation, projection, false);

	UniformLocation * textColorLocation = RenderManager::Instance()->GetUniformLocation(shader, "textColor");
	RenderManager::Instance()->SetUniform3fv(textColorLocation, colour);

	UniformLocation * textLocation = RenderManager::Instance()->GetUniformLocation(shader, "text");
	RenderManager::Instance()->UseTexture(textLocation, nullptr, 0);

	RenderManager::Instance()->BindVertexArray(VAO);

	float x = pPixelLocation.location.x;
	const float y = pPixelLocation.location.y;

	std::string::const_iterator c;

	for (c = text.begin(); c != text.end(); c++)
	{
		const Character * ch = Characters[*c];

		const float xpos = x + ch->Bearing.x * pPixelLocation.scale;
		const float ypos = y - (ch->Size.y - ch->Bearing.y) * pPixelLocation.scale;

		const float w = ch->Size.x * pPixelLocation.scale;
		const float h = ch->Size.y * pPixelLocation.scale;

		float vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
		{ xpos, ypos, 0.0, 1.0 },
		{ xpos + w, ypos, 1.0, 1.0 },

		{ xpos, ypos + h, 0.0, 0.0 },
		{ xpos + w, ypos, 1.0, 1.0 },
		{ xpos + w, ypos + h, 1.0, 0.0 }
		};


		ch->RenderCharacter();
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch->Advance >> 6) * pPixelLocation.scale;
	}

	RenderManager::Instance()->BindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, 0);
}

#pragma once

#include "TextRender.h"
#include "OpenGL.h"
#include "OpenGL330VertexBuffer.h"

struct OpenGL330Character : public Character {
	unsigned int TextureID;
	void RenderCharacter() const override
	{
		glBindTexture(GL_TEXTURE_2D, TextureID);
	}
};

class OpenGL330TextRender : public TextRender
{
private: 
	unsigned int VAO;
	unsigned int VBO;
	OpenGL330VertexBuffer * zeroBuffer = new OpenGL330VertexBuffer(0);
	OpenGL330VertexBuffer * VAOBuffer;

public:
	OpenGL330TextRender();
	~OpenGL330TextRender();

	void RenderText(const std::string & text, const PixelLocation & pPixelLocation, const glm::vec3 & colour) override;
};


#pragma once

#include "VertexBuffer.h"

class OpenGL330VertexBuffer : public VertexBuffer
{
private:
	int VAO;

public:
	OpenGL330VertexBuffer(int pVAO);
	~OpenGL330VertexBuffer();

	void UseVertexBuffer() override;
};


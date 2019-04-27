#include "OpenGL330VertexBuffer.h"
#include "OpenGL.h"

OpenGL330VertexBuffer::OpenGL330VertexBuffer(int pVAO) : VAO(pVAO)
{
}

OpenGL330VertexBuffer::~OpenGL330VertexBuffer()
{
}

//Uses vertex array buffer
void OpenGL330VertexBuffer::UseVertexBuffer()
{
	glBindVertexArray(VAO);
}
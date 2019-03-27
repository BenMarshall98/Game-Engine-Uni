#pragma once
class VertexBuffer
{
public:
	VertexBuffer();
	virtual ~VertexBuffer();

	virtual void UseVertexBuffer() = 0;
};


#pragma once
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	virtual void UseFrameBuffer() = 0;
	virtual void BindTexture() = 0;
};


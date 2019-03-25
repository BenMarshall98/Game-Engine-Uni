#pragma once
class FrameBuffer
{
public:
	FrameBuffer();
	virtual ~FrameBuffer();

	virtual void UseFrameBuffer() = 0;
	virtual void BindTexture() = 0;
};


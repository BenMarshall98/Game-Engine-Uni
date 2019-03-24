#pragma once

#include "FrameBuffer.h"

class OpenGL330PointShadowFramebuffer : public FrameBuffer
{
private:
	unsigned int pointFBO;
	unsigned int pointCubemap;

public:
	OpenGL330PointShadowFramebuffer(int width, int height);
	~OpenGL330PointShadowFramebuffer();

	void UseFrameBuffer() override;
	void BindTexture() override;
};


#pragma once
#include "FrameBuffer.h"

class OpenGL330DirectionalShadowFramebuffer : public FrameBuffer
{
private:
	unsigned int FBO;
	unsigned int Texture;
public:
	OpenGL330DirectionalShadowFramebuffer(int width, int height);
	~OpenGL330DirectionalShadowFramebuffer();

	void UseFrameBuffer() override;
	void BindTexture() override;
};


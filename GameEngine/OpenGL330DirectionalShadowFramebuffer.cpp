#include "OpenGL330DirectionalShadowFramebuffer.h"
#include "OpenGL.h"

OpenGL330DirectionalShadowFramebuffer::OpenGL330DirectionalShadowFramebuffer(const int width, const int height) :
	FBO(0), Texture(0)
{
	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


OpenGL330DirectionalShadowFramebuffer::~OpenGL330DirectionalShadowFramebuffer()
{
}

//Uses framebuffer
void OpenGL330DirectionalShadowFramebuffer::UseFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

//Uses texture
void OpenGL330DirectionalShadowFramebuffer::BindTexture()
{
	glBindTexture(GL_TEXTURE_2D, Texture);
}
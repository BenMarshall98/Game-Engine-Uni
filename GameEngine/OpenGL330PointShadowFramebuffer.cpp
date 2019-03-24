#include "OpenGL330PointShadowFramebuffer.h"
#include "OpenGL.h"

OpenGL330PointShadowFramebuffer::OpenGL330PointShadowFramebuffer(int width, int height)
{
	glGenFramebuffers(1, &pointFBO);

	glGenTextures(1, &pointCubemap);

	glBindTexture(GL_TEXTURE_CUBE_MAP, pointCubemap);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, pointFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, pointCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGL330PointShadowFramebuffer::~OpenGL330PointShadowFramebuffer()
{
}

void OpenGL330PointShadowFramebuffer::UseFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, pointFBO);
}

void OpenGL330PointShadowFramebuffer::BindTexture()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, pointCubemap);
}
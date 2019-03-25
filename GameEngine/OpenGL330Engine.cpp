#include "OpenGL330Engine.h"

#include "OpenGL.h"
#include "OpenGL330Texture.h"
#include "OpenGL330AnimatedModel.h"
#include "OpenGL330DirectionalShadowFramebuffer.h"
#include "OpenGL330PointShadowFramebuffer.h"
#include "OpenGL330Shader.h"
#include "OpenGL330StaticModel.h"

OpenGL330Engine::OpenGL330Engine()
{
}

OpenGL330Engine::~OpenGL330Engine()
{
}

Texture * OpenGL330Engine::CreateTexture(const std::string & fileName)
{
	Texture * const texture = new OpenGL330Texture();

	if (!texture->LoadTexture(fileName))
	{
		delete texture;
		return nullptr;
	}

	return texture;
}

Shader * OpenGL330Engine::CreateShader(const std::string & vertexFile, const std::string & fragmentFile, const std::string & geometryFile)
{
	Shader * const shader = new OpenGL330Shader();

	if (!shader->LoadShader(vertexFile, fragmentFile, geometryFile))
	{
		delete shader;
		return nullptr;
	}

	return shader;
}

StaticModel * OpenGL330Engine::CreateStaticModel(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec2> & pTexture, const std::vector<glm::vec3> & pNormal, const std::vector<int> & pIndices, const std::vector<glm::vec3> & pTangents)
{
	return new OpenGL330StaticModel(pVertex, pTexture, pNormal, pIndices, pTangents);
}

AnimatedModelMesh * OpenGL330Engine::CreateAnimatedModelMesh(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec3> & pNormal, const std::vector<glm::vec2> & pTextures, const std::vector<glm::vec4> & pWeights, const std::vector<glm::ivec4> & pIds, const std::vector<int> & pIndices)
{
	return new OpenGL330AnimatedModel(pVertex, pNormal, pTextures, pWeights, pIds, pIndices);
}

FrameBuffer * OpenGL330Engine::CreateDirectionShadowFrameBuffer(int width, int height)
{
	return new OpenGL330DirectionalShadowFramebuffer(width, height);
}

FrameBuffer * OpenGL330Engine::CreatePointShadowFrameBuffer(int width, int height)
{
	return new OpenGL330PointShadowFramebuffer(width, height);
}

void OpenGL330Engine::SetViewport(const int width, const int height)
{
	glViewport(0, 0, width, height);
}

void OpenGL330Engine::UseTexture(Shader * const shader, const std::string & uniformName, Texture * const texture, const int activeTexture)
{
	shader->UseTexture(uniformName, activeTexture);

	if (texture)
	{
		texture->BindTexture();
	}
}

void OpenGL330Engine::UseFrameBufferTexture(Shader * const shader, const std::string & uniformName, FrameBuffer * const frameBuffer, const int activeTexture)
{
	shader->UseTexture(uniformName, activeTexture);
	frameBuffer->BindTexture();
}

void OpenGL330Engine::ClearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL330Engine::EnableDepth()
{
	glEnable(GL_DEPTH_TEST);
}

void OpenGL330Engine::ClearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGL330Engine::ClearShader()
{
	glUseProgram(0);
}

void OpenGL330Engine::ClearFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGL330Engine::CullFace(const std::string & cullType)
{
	if (cullType == "Front")
	{
		glCullFace(GL_FRONT);
	}
	else if (cullType == "Back")
	{
		glCullFace(GL_BACK);
	}
}

void OpenGL330Engine::ClearColor(const glm::vec4 & colour)
{
	glClearColor(colour.x, colour.y, colour.z, colour.w);
}
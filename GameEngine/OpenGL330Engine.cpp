#include "OpenGL330Engine.h"

#include "OpenGL.h"
#include "OpenGL330Texture.h"
#include "OpenGL330AnimatedModel.h"
#include "OpenGL330DirectionalShadowFramebuffer.h"
#include "OpenGL330PointShadowFramebuffer.h"
#include "OpenGL330Shader.h"
#include "OpenGL330StaticModel.h"
#include "OpenGL330TextRender.h"

OpenGL330Engine::OpenGL330Engine() : RenderEngine()
{
}

OpenGL330Engine::~OpenGL330Engine()
{
}

//Creates texture
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

//Creates shader
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

//Creates static model
StaticModel * OpenGL330Engine::CreateStaticModel(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec2> & pTexture, const std::vector<glm::vec3> & pNormal, const std::vector<int> & pIndices, const std::vector<glm::vec3> & pTangents)
{
	return new OpenGL330StaticModel(pVertex, pTexture, pNormal, pIndices, pTangents);
}

//Create animated model mesh
AnimatedModelMesh * OpenGL330Engine::CreateAnimatedModelMesh(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec3> & pNormal, const std::vector<glm::vec2> & pTextures, const std::vector<glm::vec4> & pWeights, const std::vector<glm::ivec4> & pIds, const std::vector<int> & pIndices)
{
	return new OpenGL330AnimatedModel(pVertex, pNormal, pTextures, pWeights, pIds, pIndices);
}

//Creates direction shadow framebuffer
FrameBuffer * OpenGL330Engine::CreateDirectionShadowFrameBuffer(int width, int height)
{
	return new OpenGL330DirectionalShadowFramebuffer(width, height);
}

//Creates point shadow framebuffer
FrameBuffer * OpenGL330Engine::CreatePointShadowFrameBuffer(int width, int height)
{
	return new OpenGL330PointShadowFramebuffer(width, height);
}

//Sets the viewport
void OpenGL330Engine::SetViewport(const int width, const int height)
{
	glViewport(0, 0, width, height);
}

//Uses texture
void OpenGL330Engine::UseTexture(UniformLocation * const location, Texture * const texture, const int activeTexture)
{
	location->UseTexture(activeTexture);

	if (texture)
	{
		texture->BindTexture();
	}
}

//Uses framebuffer texture
void OpenGL330Engine::UseFrameBufferTexture(UniformLocation * const location, FrameBuffer * const frameBuffer, const int activeTexture)
{
	location->UseTexture(activeTexture);
	frameBuffer->BindTexture();
}

//Clears the colour buffer
void OpenGL330Engine::ClearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

//Enable depth
void OpenGL330Engine::EnableDepth()
{
	glEnable(GL_DEPTH_TEST);
}

//Clears the depth buffer
void OpenGL330Engine::ClearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

//Clears the shader program
void OpenGL330Engine::ClearShader()
{
	glUseProgram(0);
}

//Clears the framebuffer used
void OpenGL330Engine::ClearFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//Sets cull face
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

//Sets the clear colour
void OpenGL330Engine::ClearColor(const glm::vec4 & colour)
{
	glClearColor(colour.x, colour.y, colour.z, colour.w);
}

//Binds vertex array
void OpenGL330Engine::BindVertexArray(VertexBuffer * VAO)
{
	static VertexBuffer * lastVAO = nullptr;

	if (lastVAO != VAO)
	{
		VAO->UseVertexBuffer();
		lastVAO = VAO;
	}
}

void OpenGL330Engine::InitialiseTextRender()
{
	textRender = new OpenGL330TextRender();
}
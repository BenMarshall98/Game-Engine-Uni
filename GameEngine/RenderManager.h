#pragma once

#include "RenderEngine.h"

class RenderManager
{
private:
	static RenderManager * instance;

	static RenderEngine * engine;

	RenderManager();
public:
	
	static inline  RenderManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new RenderManager();
		}
		return instance;
	}

	~RenderManager();

	inline Texture * CreateTexture(std::string fileName)
	{
		return engine->CreateTexture(fileName);
	}

	inline Shader * CreateShader(std::string vertexFile, std::string fragmentFile, std::string geometryFile)
	{
		return engine->CreateShader(vertexFile, fragmentFile, geometryFile);
	}

	inline StaticModel * CreateStaticModel(std::vector<glm::vec3> & pVertex, std::vector<glm::vec2> & pTexture, std::vector<glm::vec3> & pNormal, std::vector<int> & pIndices, std::vector<glm::vec3> & pTangents)
	{
		return engine->CreateStaticModel(pVertex, pTexture, pNormal, pIndices, pTangents);
	}

	inline AnimatedModelMesh * CreateAnimatedModelMesh(std::vector<glm::vec3> & pVertex, std::vector<glm::vec3> & pNormal, std::vector<glm::vec2> & pTextures, std::vector<glm::vec4> & pWeights, std::vector<glm::ivec4> & pIds, std::vector<int> pIndices)
	{
		return engine->CreateAnimatedModelMesh(pVertex, pNormal, pTextures, pWeights, pIds, pIndices);
	}

	inline FrameBuffer * CreateDirectionShadowFrameBuffer(int width, int height)
	{
		return engine->CreateDirectionShadowFrameBuffer(width, height);
	}

	inline FrameBuffer * CreatePointShadowFrameBuffer(int width, int height)
	{
		return engine->CreatePointShadowFrameBuffer(width, height);
	}

	inline void SetViewport(int width, int height)
	{
		engine->SetViewport(width, height);
	}

	inline void SetUniform1i(Shader * shader, std::string uniformName, int value)
	{
		shader->SetUniform1i(uniformName, value);
	}

	inline void SetUniform1f(Shader * shader, std::string uniformName, float value)
	{
		shader->SetUniform1f(uniformName, value);
	}

	inline void SetUniform3fv(Shader * shader, std::string uniformName, glm::vec3 value)
	{
		shader->SetUniform3fv(uniformName, value);
	}

	inline void SetUniformMatrix4fv(Shader * shader, std::string uniformName, glm::mat4 value, bool transpose)
	{
		shader->SetUniformMatrix4fv(uniformName, value, transpose);
	}

	inline void UseTexture(Shader * shader, std::string uniformName, Texture * texture, int activeTexture)
	{
		engine->UseTexture(shader, uniformName, texture, activeTexture);
	}

	inline void UseFrameBufferTexture(Shader * shader, std::string uniformName, FrameBuffer * framebuffer, int activeTexture)
	{
		engine->UseFrameBufferTexture(shader, uniformName, framebuffer, activeTexture);
	}

	inline void UseFrameBuffer(FrameBuffer * frameBuffer)
	{
		frameBuffer->UseFrameBuffer();
	}

	inline void ClearColorBuffer()
	{
		engine->ClearColorBuffer();
	}

	inline void EnableDepth()
	{
		engine->EnableDepth();
	}

	inline void ClearDepthBuffer()
	{
		engine->ClearDepthBuffer();
	}

	inline void ClearShader()
	{
		engine->ClearShader();
	}

	inline void ClearFrameBuffer()
	{
		engine->ClearFrameBuffer();
	}

	inline void CullFace(std::string cullType)
	{
		engine->CullFace(cullType);
	}

	inline void ClearColor(glm::vec3 colour)
	{
		engine->ClearColor(glm::vec4(colour, 1.0f));
	}

	inline void ClearColor(glm::vec4 colour)
	{
		engine->ClearColor(colour);
	}
};


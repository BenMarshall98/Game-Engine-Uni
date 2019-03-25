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

	inline Texture * CreateTexture(const std::string & fileName)
	{
		return engine->CreateTexture(fileName);
	}

	inline Shader * CreateShader(const std::string & vertexFile, const std::string & fragmentFile, const std::string & geometryFile)
	{
		return engine->CreateShader(vertexFile, fragmentFile, geometryFile);
	}

	inline StaticModel * CreateStaticModel(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec2> & pTexture, const std::vector<glm::vec3> & pNormal, const std::vector<int> & pIndices, const std::vector<glm::vec3> & pTangents)
	{
		return engine->CreateStaticModel(pVertex, pTexture, pNormal, pIndices, pTangents);
	}

	inline AnimatedModelMesh * CreateAnimatedModelMesh(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec3> & pNormal, const std::vector<glm::vec2> & pTextures, const std::vector<glm::vec4> & pWeights, const std::vector<glm::ivec4> & pIds, const std::vector<int> & pIndices)
	{
		return engine->CreateAnimatedModelMesh(pVertex, pNormal, pTextures, pWeights, pIds, pIndices);
	}

	inline FrameBuffer * CreateDirectionShadowFrameBuffer(const int width, const int height)
	{
		return engine->CreateDirectionShadowFrameBuffer(width, height);
	}

	inline FrameBuffer * CreatePointShadowFrameBuffer(const int width, const int height)
	{
		return engine->CreatePointShadowFrameBuffer(width, height);
	}

	inline void SetViewport(const int width, const int height)
	{
		engine->SetViewport(width, height);
	}

	inline void SetUniform1i(Shader * const shader, const std::string & uniformName, const int value)
	{
		shader->SetUniform1i(uniformName, value);
	}

	inline void SetUniform1f(Shader * const shader, const std::string & uniformName, const float value)
	{
		shader->SetUniform1f(uniformName, value);
	}

	inline void SetUniform3fv(Shader * const shader, const std::string & uniformName, const glm::vec3 & value)
	{
		shader->SetUniform3fv(uniformName, value);
	}

	inline void SetUniformMatrix4fv(Shader * const shader, const std::string & uniformName, const glm::mat4 & value, bool transpose)
	{
		shader->SetUniformMatrix4fv(uniformName, value, transpose);
	}

	inline void UseTexture(Shader * const shader, const std::string & uniformName, Texture * const texture, const int activeTexture)
	{
		engine->UseTexture(shader, uniformName, texture, activeTexture);
	}

	inline void UseFrameBufferTexture(Shader * const shader, const std::string & uniformName, FrameBuffer * const framebuffer, const int activeTexture)
	{
		engine->UseFrameBufferTexture(shader, uniformName, framebuffer, activeTexture);
	}

	inline void UseFrameBuffer(FrameBuffer * const frameBuffer)
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

	inline void CullFace(const std::string & cullType)
	{
		engine->CullFace(cullType);
	}

	inline void ClearColor(const glm::vec3 & colour)
	{
		engine->ClearColor(glm::vec4(colour, 1.0f));
	}

	inline void ClearColor(const glm::vec4 & colour)
	{
		engine->ClearColor(colour);
	}
};


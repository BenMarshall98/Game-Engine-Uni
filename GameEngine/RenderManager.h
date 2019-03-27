#pragma once

#include "RenderEngine.h"
#include "OpenGL330Engine.h"

class RenderManager
{
private:
	static std::string engineName;

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

		if (engine == nullptr)
		{
			if (engineName == "OpenGL330")
			{
				engine = new OpenGL330Engine();
				
			}

			engine->InitialiseTextRender();
		}

		return instance;
	}

	~RenderManager();

	static void SetRenderEngine(std::string pEngineName)
	{
		engineName = pEngineName;
	}

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

	inline UniformLocation * GetUniformLocation(Shader * shader, const std::string & uniformName)
	{
		return shader->GetUniformLocation(uniformName);
	}

	inline void SetViewport(const int width, const int height)
	{
		engine->SetViewport(width, height);
	}

	inline void SetUniform1i(UniformLocation * const location,  const int value)
	{
		location->SetUniform1i(value);
	}

	inline void SetUniform1f(UniformLocation * const location, const float value)
	{
		location->SetUniform1f(value);
	}

	inline void SetUniform3fv(UniformLocation * const location, const glm::vec3 & value)
	{
		location->SetUniform3fv(value);
	}

	inline void SetUniformMatrix4fv(UniformLocation * const location, const glm::mat4 & value, bool transpose)
	{
		location->SetUniformMatrix4fv(value, transpose);
	}

	inline void UseTexture(UniformLocation * const location, Texture * const texture, const int activeTexture)
	{
		engine->UseTexture(location, texture, activeTexture);
	}

	inline void UseFrameBufferTexture(UniformLocation * const location, FrameBuffer * const framebuffer, const int activeTexture)
	{
		engine->UseFrameBufferTexture(location, framebuffer, activeTexture);
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

	inline void BindVertexArray(VertexBuffer * VAO)
	{
		engine->BindVertexArray(VAO);
	}

	inline void RenderText(const std::string & text, const PixelLocation & pPixelLocation, const glm::vec3 & colour)
	{
		engine->RenderText(text, pPixelLocation, colour);
	}

	inline PixelLocation CalculateTextSize(const std::string & text, const glm::vec2 & location, float size, const std::string & align)
	{
		return engine->CalculateTextSize(text, location, size, align);
	}
};


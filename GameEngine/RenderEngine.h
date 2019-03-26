#pragma once

#include <string>
#include "StaticModel.h"
#include "AnimatedModelMesh.h"
#include "Texture.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "glm/glm.hpp"
#include "TextRender.h"
#include <vector>

class RenderEngine
{
protected:
	TextRender * textRender = nullptr;

public:
	RenderEngine();
	virtual ~RenderEngine();

	virtual Texture * CreateTexture(const std::string & fileName) = 0;
	virtual Shader * CreateShader(const std::string & vertexFile, const std::string & fragmentFile, const std::string & geometryFile) = 0;
	virtual StaticModel * CreateStaticModel(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec2> & pTexture, const std::vector<glm::vec3> & pNormal, const std::vector<int> & pIndices, const std::vector<glm::vec3> & pTangents) = 0;
	virtual AnimatedModelMesh * CreateAnimatedModelMesh(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec3> & pNormal, const std::vector<glm::vec2> & pTextures, const std::vector<glm::vec4> & pWeights, const std::vector<glm::ivec4> & pIds, const std::vector<int> & pIndices) = 0;
	virtual FrameBuffer * CreateDirectionShadowFrameBuffer(int width, int height) = 0;
	virtual FrameBuffer * CreatePointShadowFrameBuffer(int width, int height) = 0;

	virtual void SetViewport(int width, int height) = 0;

	virtual void UseTexture(UniformLocation * location,Texture * texture, int activeTexture) = 0;
	virtual void UseFrameBufferTexture(UniformLocation * location, FrameBuffer * frameBuffer, int activeTexture) = 0;
	virtual void ClearColorBuffer() = 0;
	virtual void EnableDepth() = 0;
	virtual void ClearDepthBuffer() = 0;
	virtual void ClearShader() = 0;
	virtual void ClearFrameBuffer() = 0;
	virtual void CullFace(const std::string & cullType) = 0;
	virtual void ClearColor(const glm::vec4 & colour) = 0;
	virtual void BindVertexArray(int VAO) = 0;
	virtual void InitialiseTextRender() = 0;

	inline void RenderText(const std::string & text, const PixelLocation & pPixelLocation, const glm::vec3 & colour)
	{
		if (textRender == nullptr)
		{
			InitialiseTextRender();
		}
		textRender->RenderText(text, pPixelLocation, colour);
	}

	inline PixelLocation CalculateTextSize(const std::string & text, const glm::vec2 & location, float size, const std::string & align)
	{
		if (textRender == nullptr)
		{
			InitialiseTextRender();
		}
		return textRender->CalculateSize(text, location, size, align);
	}
};


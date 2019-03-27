#pragma once

#include "RenderEngine.h"

class OpenGL330Engine : public RenderEngine
{
public:
	OpenGL330Engine();
	~OpenGL330Engine();

	Texture * CreateTexture(const std::string & fileName) override;
	Shader * CreateShader(const std::string & vertexFile, const std::string & fragmentFile, const std::string & geometryFile) override;
	StaticModel * CreateStaticModel(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec2> & pTexture, const std::vector<glm::vec3> & pNormal, const std::vector<int> & pIndices, const std::vector<glm::vec3> & pTangents) override;
	AnimatedModelMesh * CreateAnimatedModelMesh(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec3> & pNormal, const std::vector<glm::vec2> & pTextures, const std::vector<glm::vec4> & pWeights, const std::vector<glm::ivec4> & pIds, const std::vector<int> & pIndices) override;
	FrameBuffer * CreateDirectionShadowFrameBuffer(int width, int height) override;
	FrameBuffer * CreatePointShadowFrameBuffer(int width, int height) override;

	void SetViewport(int width, int height) override;

	void UseTexture(UniformLocation * location, Texture * texture, int activeTexture) override;
	void UseFrameBufferTexture(UniformLocation * location, FrameBuffer * frameBuffer, int activeTexture) override;
	void ClearColorBuffer() override;
	void EnableDepth() override;
	void ClearDepthBuffer() override;
	void ClearShader() override;
	void ClearFrameBuffer() override;
	void CullFace(const std::string & cullType) override;
	void ClearColor(const glm::vec4 & colour) override;
	void BindVertexArray(VertexBuffer * VAO) override;
	void InitialiseTextRender() override;
};


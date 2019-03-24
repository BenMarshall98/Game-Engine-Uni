#pragma once

#include "RenderEngine.h"

class OpenGL330Engine : public RenderEngine
{
public:
	OpenGL330Engine();
	~OpenGL330Engine();

	Texture * CreateTexture(std::string fileName) override;
	Shader * CreateShader(std::string vertexFile, std::string fragmentFile, std::string geometryFile) override;
	StaticModel * CreateStaticModel(std::vector<glm::vec3> & pVertex, std::vector<glm::vec2> & pTexture, std::vector<glm::vec3> & pNormal, std::vector<int> & pIndices, std::vector<glm::vec3> & pTangents) override;
	AnimatedModelMesh * CreateAnimatedModelMesh(std::vector<glm::vec3> & pVertex, std::vector<glm::vec3> & pNormal, std::vector<glm::vec2> & pTextures, std::vector<glm::vec4> & pWeights, std::vector<glm::ivec4> & pIds, std::vector<int> pIndices) override;
	FrameBuffer * CreateDirectionShadowFrameBuffer(int width, int height) override;
	FrameBuffer * CreatePointShadowFrameBuffer(int width, int height) override;

	void SetViewport(int width, int height) override;

	void UseTexture(Shader * shader, std::string uniformName, Texture * texture, int activeTexture) override;
	void UseFrameBufferTexture(Shader * shader, std::string uniformName, FrameBuffer * frameBuffer, int activeTexture) override;
	void ClearColorBuffer() override;
	void EnableDepth() override;
	void ClearDepthBuffer() override;
	void ClearShader() override;
	void ClearFrameBuffer() override;
	void CullFace(std::string cullType) override;
	void ClearColor(glm::vec4 colour) override;
};


#pragma once

#include <string>
#include "StaticModel.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "glm/glm.hpp"
#include <vector>

class RenderEngine
{
public:
	RenderEngine();
	~RenderEngine();

	virtual Texture * CreateTexture(std::string fileName) = 0;
	virtual Shader * CreateShader(std::string vertexFile, std::string fragmentFile, std::string geometryFile = "") = 0;
	virtual StaticModel * CreateStaticModel(std::vector<glm::vec3> & pVertex, std::vector<glm::vec2> & pTexture, std::vector<glm::vec3> & pNormal, std::vector<int> & pIndices, std::vector<glm::vec3> & pTangents) = 0;
	virtual Mesh * CreateAnimatedModelMesh(std::vector<glm::vec3> & pVertex, std::vector<glm::vec3> & pNormal, std::vector<glm::vec2> & pTextures, std::vector<glm::vec4> & pWeights, std::vector<glm::ivec4> & pIds, std::vector<int> pIndices) = 0;
	virtual FrameBuffer * CreateDirectionShadowFrameBuffer() = 0;
	virtual FrameBuffer * CreatePointShadowFrameBuffer() = 0;

	virtual void SetViewport(int width, int height) = 0;

};


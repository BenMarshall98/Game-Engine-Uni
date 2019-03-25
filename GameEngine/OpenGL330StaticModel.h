#pragma once

#include "StaticModel.h"

class OpenGL330StaticModel : public StaticModel
{
private:
	unsigned int VAO;
	unsigned int EBO;
	unsigned int VBO[5];
public:
	OpenGL330StaticModel(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec2> & pTexture, const std::vector<glm::vec3> & pNormal, const std::vector<int> & pIndices, const std::vector<glm::vec3> & pTangents);
	~OpenGL330StaticModel();

	OpenGL330StaticModel& operator=(const OpenGL330StaticModel&) = delete;
	OpenGL330StaticModel(OpenGL330StaticModel&) = delete;

	void Render(Shader * const shader) override;
	void Render(Shader * shader, const std::vector<glm::mat4> & boneMats) override;
};


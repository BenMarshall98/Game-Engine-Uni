#pragma once

#include "StaticModel.h"

class OpenGL330StaticModel : public StaticModel
{
private:
	unsigned int VAO;
	unsigned int EBO;
	unsigned int VBO[5];
public:
	OpenGL330StaticModel(std::vector<glm::vec3> & pVertex, std::vector<glm::vec2> & pTexture, std::vector<glm::vec3> & pNormal, std::vector<int> & pIndices, std::vector<glm::vec3> & pTangents);
	~OpenGL330StaticModel();

	void Render(Shader * const shader) override;
	void Render(Shader * shader, std::vector<glm::mat4> boneMats) override;
};


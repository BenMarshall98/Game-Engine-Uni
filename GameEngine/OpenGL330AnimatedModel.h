#pragma once

#include "AnimatedModelMesh.h"
class OpenGL330AnimatedModel : public AnimatedModelMesh
{
private:
	unsigned int VAO, VBO[5], EBO;

public:
	OpenGL330AnimatedModel(std::vector<glm::vec3> & pVertex, std::vector<glm::vec3> & pNormal, std::vector<glm::vec2> & pTextures, std::vector<glm::vec4> & pWeights, std::vector<glm::ivec4> & pIds, std::vector<int> pIndices);
	~OpenGL330AnimatedModel();

	void Render() override;
};


#pragma once

#include "AnimatedModelMesh.h"
class OpenGL330AnimatedModel : public AnimatedModelMesh
{
private:
	unsigned int VAO, VBO[5], EBO;

public:
	OpenGL330AnimatedModel(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec3> & pNormal, const std::vector<glm::vec2> & pTextures, const std::vector<glm::vec4> & pWeights, const std::vector<glm::ivec4> & pIds, const std::vector<int> & pIndices);
	~OpenGL330AnimatedModel();

	OpenGL330AnimatedModel& operator=(const OpenGL330AnimatedModel&) = delete;
	OpenGL330AnimatedModel(OpenGL330AnimatedModel&) = delete;

	void Render() override;
};


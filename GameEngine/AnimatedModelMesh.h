#pragma once

#include "glm/glm.hpp"
#include <vector>

class AnimatedModelMesh
{
protected:
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec4> weights;
	std::vector<glm::ivec4> ids;
	std::vector<int> indices;

public:
	AnimatedModelMesh(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec3> & pNormal, const std::vector<glm::vec2> & pTextures, const std::vector<glm::vec4> & pWeights, const std::vector<glm::ivec4> & pIds, const std::vector<int> & pIndices);
	virtual ~AnimatedModelMesh();

	AnimatedModelMesh& operator=(const AnimatedModelMesh&) = delete;
	AnimatedModelMesh(AnimatedModelMesh&) = delete;

	virtual void Render() = 0;
};

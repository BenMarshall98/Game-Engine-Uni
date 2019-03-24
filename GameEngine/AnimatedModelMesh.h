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
	AnimatedModelMesh(std::vector<glm::vec3> & pVertex, std::vector<glm::vec3> & pNormal, std::vector<glm::vec2> & pTextures, std::vector<glm::vec4> & pWeights, std::vector<glm::ivec4> & pIds, std::vector<int> pIndices);
	~AnimatedModelMesh();

	AnimatedModelMesh& operator=(const AnimatedModelMesh&) = delete;
	AnimatedModelMesh(AnimatedModelMesh&) = delete;

	virtual void Render() = 0;
};

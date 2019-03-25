#include "AnimatedModelMesh.h"

AnimatedModelMesh::AnimatedModelMesh(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec3> & pNormal, const std::vector<glm::vec2> & pTextures, const std::vector<glm::vec4> & pWeights, const std::vector<glm::ivec4> & pIds, const std::vector<int> & pIndices) :
	vertex(pVertex), normal(pNormal), textures(pTextures), weights(pWeights), ids(pIds), indices(pIndices)
{
}

AnimatedModelMesh::~AnimatedModelMesh()
{
	
}

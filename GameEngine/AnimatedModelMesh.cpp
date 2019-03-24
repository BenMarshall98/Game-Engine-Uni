#include "AnimatedModelMesh.h"

AnimatedModelMesh::AnimatedModelMesh(std::vector<glm::vec3> & pVertex, std::vector<glm::vec3> & pNormal, std::vector<glm::vec2> & pTextures, std::vector<glm::vec4> & pWeights, std::vector<glm::ivec4> & pIds, std::vector<int> pIndices) :
	vertex(pVertex), normal(pNormal), textures(pTextures), weights(pWeights), ids(pIds), indices(pIndices)
{
}

AnimatedModelMesh::~AnimatedModelMesh()
{
	
}

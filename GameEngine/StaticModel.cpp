#include "StaticModel.h"

StaticModel::StaticModel(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec2> & pTexture, const std::vector<glm::vec3> & pNormal, const std::vector<int> & pIndices, const std::vector<glm::vec3> & pTangents/*, vector<vec3> & pBitangents*/) :
	mVertex(pVertex), mTexture(pTexture), mNormal(pNormal), mTangents(pTangents)/*, mBitangents(pBitangents)*/, mIndices(pIndices)
{
}

StaticModel::~StaticModel()
{
}

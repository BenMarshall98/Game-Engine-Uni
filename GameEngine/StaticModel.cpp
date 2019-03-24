#include "StaticModel.h"

StaticModel::StaticModel(std::vector<glm::vec3> & pVertex, std::vector<glm::vec2> & pTexture, std::vector<glm::vec3> & pNormal, std::vector<int> & pIndices, std::vector<glm::vec3> & pTangents/*, vector<vec3> & pBitangents*/) :
	mVertex(pVertex), mTexture(pTexture), mNormal(pNormal), mTangents(pTangents)/*, mBitangents(pBitangents)*/, mIndices(pIndices)
{
}

StaticModel::~StaticModel()
{
}

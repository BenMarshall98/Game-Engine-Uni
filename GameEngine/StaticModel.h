#pragma once
#include "iModel.h"
#include "glm/glm.hpp"

#include <vector>

class StaticModel : public iModel
{
protected:
	std::vector<glm::vec3> mVertex;
	std::vector<glm::vec2> mTexture;
	std::vector<glm::vec3> mNormal;
	std::vector<glm::vec3> mTangents;
	/*vector<vec3> mBitangents;*/
	std::vector<int> mIndices;
	
public:
	StaticModel(const std::vector<glm::vec3> & pVertex, const std::vector<glm::vec2> & pTexture, const std::vector<glm::vec3> & pNormal, const std::vector<int> & pIndices, const std::vector<glm::vec3> & pTangents/*, vector<vec3> & pBitangents*/);

	StaticModel& operator= (const StaticModel&) = delete;
	StaticModel(StaticModel&) = delete;

	~StaticModel();
};


#pragma once
#include "iModel.h"
#include "glm/glm.hpp"

#include <vector>

class StaticModel : public iModel
{
private:
	std::vector<glm::vec3> mVertex;
	std::vector<glm::vec2> mTexture;
	std::vector<glm::vec3> mNormal;
	std::vector<glm::vec3> mTangents;
	/*vector<vec3> mBitangents;*/
	std::vector<int> mIndices;

	unsigned int VBO[5];
	unsigned int VAO, EBO;
	
public:
	StaticModel(std::vector<glm::vec3> & pVertex, std::vector<glm::vec2> & pTexture, std::vector<glm::vec3> & pNormal, std::vector<int> & pIndices, std::vector<glm::vec3> & pTangents/*, vector<vec3> & pBitangents*/);

	StaticModel& operator= (const StaticModel&) = delete;
	StaticModel(StaticModel&) = delete;

	~StaticModel();
	void Render(Shader * shader) override;
	void Render(Shader * shader, std::vector<glm::mat4> boneMats);
};


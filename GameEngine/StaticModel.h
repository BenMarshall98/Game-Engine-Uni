#pragma once
#include "iModel.h"
#include "glm/glm.hpp"

#include <vector>

using namespace std;
using namespace glm;

class StaticModel : public iModel
{
private:
	vector<vec3> mVertex;
	vector<vec2> mTexture;
	vector<vec3> mNormal;
	vector<vec3> mTangents;
	/*vector<vec3> mBitangents;*/
	vector<int> mIndices;

	unsigned int VBO[5];
	unsigned int VAO, EBO;
	
public:
	StaticModel(vector<vec3> & pVertex, vector<vec2> & pTexture, vector<vec3> & pNormal, vector<int> & pIndices, vector<vec3> & pTangents/*, vector<vec3> & pBitangents*/);

	StaticModel& operator= (const StaticModel&) = delete;
	StaticModel(StaticModel&) = delete;

	~StaticModel();
	void Render(Shader * shader) override;
};


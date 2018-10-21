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
	vector<int> mIndices;

	unsigned int VAO, EBO;
	unsigned int VBO[3];
public:
	StaticModel(vector<vec3> & pVertex, vector<vec2> & pTexture, vector<vec3> & pNormal, vector<int> & pIndices);
	~StaticModel();
	void Render(void) override;
};


#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include <vector>

using namespace glm;
using namespace std;

class Mesh
{
private:
	vector<vec3> vertex;
	vector<vec3> normal;
	vector<vec2> textures;
	vector<vec4> weights;
	vector<ivec4> ids;
	vector<int> indices;
	unsigned int VAO, VBO[5], EBO;

public:
	Mesh(vector<vec3> pVertex, vector<vec3> pNormal, vector<vec2> pTextures, vector<vec4> pWeights, vector<ivec4> pIds, vector<int> pIndices);
	~Mesh();

	void Render();
};

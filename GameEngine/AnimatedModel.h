#pragma once
#include "iModel.h"
#include <vector>
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

struct Mesh
{
	vector<vec3> vertex;
	vector<vec3> normal;
	vector<vec2> textures;
	vector<vec4> weights;
	vector<unsigned int[4]> ids;
	vector<int> indices;

	unsigned int VAO, VBO[3], EBO;
};

class AnimatedModel : public iModel
{
private:
	vector<Mesh> meshes;

public:
	AnimatedModel() {}
	~AnimatedModel();

	inline void AddMesh(Mesh mesh)
	{
		meshes.push_back(mesh);
	}

	void Render(Shader * shader) const override;
	void LoadModel();
};
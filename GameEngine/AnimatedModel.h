#pragma once
#include "iModel.h"
#include "glm/glm.hpp"
#include "assimp/scene.h"
#include <vector>
#include <string>

using namespace std;
using namespace glm;
using namespace Assimp;

struct Bone
{
	string name;
	aiNode* node;
	aiNodeAnim* animNode;
	Bone* parent;
	mat4 parentTransforms;
	mat4 offsetMatrix;
};

class AnimatedModel : public iModel
{
private:
	unsigned int VAO, EBO;
	unsigned int VBO[3];

public:
	vector<vec3> vertex;
	vector<vec2> texture;
	vector<vec3> normal;

	vector<int> indices;

	vector<float> weight;
	vector<unsigned int> ID;

	vector<Bone*> bones;

	vector<aiNode*> nodes;
	vector<aiNodeAnim*> nodesAnim;

	mat4 globalInverse;

//Make above privates
	AnimatedModel();
	~AnimatedModel();

	void Load();

	virtual void Render()
	{

	}
};


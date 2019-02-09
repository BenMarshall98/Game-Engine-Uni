#pragma once
#include "iModel.h"
#include <vector>
#include "glm/glm.hpp"

#include "assimp/scene.h"

using namespace Assimp;

using namespace std;
using namespace glm;

struct Mesh
{
	vector<vec3> vertex;
	vector<vec3> normal;
	vector<vec2> textures;
	vector<vec4> weights;
	vector<ivec4> ids;
	vector<int> indices;

	unsigned int VAO, VBO[5], EBO;
};

struct Animation
{
	string name;
	vector<aiNodeAnim*> animNodes;
};

struct Bone
{
	Bone() {}
	string name;
	Mesh mesh;
	aiNode * node;
	aiNodeAnim * animNode;
	Bone * parent_bone;
	mat4 parent_transforms;
	mat4 offset_matrix;
};

class AnimatedModel : public iModel
{
private:
	vector<Mesh> meshes;
	vector<Animation> animations;
	vector<Bone *> bones;

public:
	AnimatedModel() {}
	~AnimatedModel();

	inline void AddMesh(Mesh mesh)
	{
		meshes.push_back(mesh);
	}

	inline void SetBones(vector<Bone *> pBones)
	{
		bones = pBones;
	}

	inline Mesh GetMesh(int loc)
	{
		return meshes.at(loc);
	}

	inline void AddAnimation(Animation animation)
	{
		animations.push_back(animation);
	}

	inline Animation GetFirstAnimation()
	{
		return animations.at(0);
	}

	void Render(Shader * shader) const override;
	void LoadModel();

	static aiMatrix4x4 GLMMat4ToAi(mat4 mat);
	static mat4 AiToGLMMat4(aiMatrix4x4& mat);

	mat4 GetBoneParentTransforms(Bone * bone);
};
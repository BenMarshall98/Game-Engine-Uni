#pragma once
#include "iModel.h"
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "assimp/scene.h"
#include "Mesh.h"
#include "Animation.h"
#include "Bone.h"

using namespace Assimp;
using namespace std;
using namespace glm;

class AnimatedModel : public iModel
{
private:
	vector<Mesh *> meshes;
	vector<Animation> animations;
	vector<Bone *> bones;
	vector<mat4> boneMats;
	vector<aiNode *> nodes;
	mat4 globalInverse;

	float time = 0;

public:
	AnimatedModel() {}
	~AnimatedModel();

	inline void AddMesh(Mesh * mesh)
	{
		meshes.push_back(mesh);
	}

	inline void SetBones(vector<Bone *> pBones)
	{
		bones = pBones;
	}

	inline void SetNodes(vector<aiNode *> pNodes)
	{
		nodes = pNodes;
	}
	inline void SetGlobalInverse(mat4 pGlobalInverse)
	{
		globalInverse = pGlobalInverse;
	}

	inline Mesh * GetMesh(int loc)
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

	void Render(Shader * shader)override;

	static aiMatrix4x4 GLMMat4ToAi(mat4 mat);
	static mat4 AiToGLMMat4(aiMatrix4x4& mat);

	Bone * FindBone(string name);

	void UpdateBoneMatsVector();

	void Update();

};
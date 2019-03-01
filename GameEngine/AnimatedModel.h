#pragma once
#include "iModel.h"
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Mesh.h"
#include "Animation.h"
#include "Bone.h"

class AnimatedModel : public iModel
{
private:
	std::vector<Mesh *> meshes;
	std::vector<Animation *> animations;
	std::vector<Bone *> bones;
	std::vector<glm::mat4> boneMats;
	std::vector<Node *> nodes;
	std::vector<std::string> bonesLoc;
	glm::mat4 globalInverse;

	float time = 0;

public:
	AnimatedModel()
	{
		for (int i = 0; i < 100; i++)
		{
			std::string bone = "Bones[" + std::to_string(i) + ']';
			bonesLoc.push_back(bone);
		}
	}
	~AnimatedModel();

	inline void AddMesh(Mesh * mesh)
	{
		meshes.push_back(mesh);
	}

	inline void SetBones(std::vector<Bone *> pBones)
	{
		bones = pBones;
	}

	inline void SetGlobalInverse(glm::mat4 pGlobalInverse)
	{
		globalInverse = pGlobalInverse;
	}

	inline void SetNodes(std::vector<Node *> pNodes)
	{
		nodes = pNodes;
	}

	inline Mesh * GetMesh(int loc)
	{
		return meshes.at(loc);
	}

	inline void AddAnimation(Animation * animation)
	{
		animations.push_back(animation);
	}

	inline Animation * GetFirstAnimation()
	{
		return animations.at(0);
	}

	void Render(Shader * shader)override;

	Bone * FindBone(std::string name);

	void UpdateBoneMatsVector();

	void Update();

};
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
			const std::string bone = "Bones[" + std::to_string(i) + ']';
			bonesLoc.push_back(bone);
		}
	}
	~AnimatedModel();

	inline void AddMesh(Mesh * const mesh)
	{
		meshes.push_back(mesh);
	}

	inline void SetBones(const std::vector<Bone *> pBones)
	{
		bones = pBones;
	}

	inline void SetGlobalInverse(const glm::mat4 pGlobalInverse)
	{
		globalInverse = pGlobalInverse;
	}

	inline void SetNodes(const std::vector<Node *> pNodes)
	{
		nodes = pNodes;
	}

	inline Mesh * GetMesh(const int loc)
	{
		return meshes.at(loc);
	}

	inline void AddAnimation(Animation * const animation)
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
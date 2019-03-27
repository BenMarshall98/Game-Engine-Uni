#pragma once
#include "iModel.h"
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "AnimatedModelMesh.h"
#include "Animation.h"
#include "Bone.h"

class AnimatedModel : public iModel
{
private:
	std::vector<AnimatedModelMesh *> meshes;
	std::vector<Animation *> animations;
	std::vector<Bone *> bones;
	std::vector<Node *> nodes;
	std::vector<std::string> bonesLoc;
	glm::mat4 globalInverse;

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

	AnimatedModel& operator=(const AnimatedModel&) = delete;
	AnimatedModel(AnimatedModel&) = delete;

	inline void AddMesh(AnimatedModelMesh * const mesh)
	{
		meshes.push_back(mesh);
	}

	inline void SetBones(const std::vector<Bone *> & pBones)
	{
		bones = pBones;
	}

	inline void SetGlobalInverse(const glm::mat4 & pGlobalInverse)
	{
		globalInverse = pGlobalInverse;
	}

	inline void SetNodes(const std::vector<Node *> & pNodes)
	{
		nodes = pNodes;
	}

	inline AnimatedModelMesh * GetMesh(const int loc)
	{
		return meshes.at(loc);
	}

	inline void AddAnimation(Animation * const animation)
	{
		animations.push_back(animation);
	}

	inline std::vector<Animation *> GetAnimations()
	{
		return animations;
	}

	void Render(Shader * shader) override;
	void Render(Shader * shader, const std::vector<glm::mat4> & boneMats) override;

	Bone * FindBone(const std::string & name);

	std::vector<glm::mat4> UpdateBoneMatsVector();

	std::vector<glm::mat4> Update(const std::string & animation, float & time, float timePeriod);

};
#define GLM_ENABLE_EXPERIMENTAL

#include "AnimatedModel.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "RenderManager.h"

void AnimatedModel::Render(Shader * shader)
{
}

//Renders the animated model
void AnimatedModel::Render(Shader * shader, const std::vector<glm::mat4> & boneMats)
{
	for (int i = 0; i < boneMats.size(); i++)
	{
		UniformLocation * location = RenderManager::Instance()->GetUniformLocation(shader, bonesLoc.at(i));
		RenderManager::Instance()->SetUniformMatrix4fv(location, boneMats[i], true);
	}

	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i)->Render();
	}
}

//Finds a bone by name
Bone * AnimatedModel::FindBone(const std::string & name)
{
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones.at(i)->GetName() == name)
		{
			return bones.at(i);
		}
	}

	return nullptr;
}

//Calculates the bone matrices
std::vector<glm::mat4> AnimatedModel::UpdateBoneMatsVector()
{
	std::vector<glm::mat4> boneMats = std::vector<glm::mat4>();

	for (int i = 0; i < 100; i++)
	{
		const int size = bones.size() - 1;
		if (i > size)
		{
			boneMats.push_back(glm::mat4(1.0));
		}
		else
		{
			const glm::mat4 transform = bones.at(i)->GetNode()->GetTransform();
			glm::mat4 tmp = glm::mat4(1.0);
			tmp *= bones.at(i)->GetOffsetMatrix();
			tmp *= transform;
			tmp *= bones.at(i)->GetParentTransforms();
			tmp *= globalInverse;

			boneMats.push_back(tmp);
		}
	}

	return boneMats;
}

//Updates the animation model, with the given animation
std::vector<glm::mat4> AnimatedModel::Update(const std::string & animationName, float & time, float timePeriod)
{
	Animation * animation = animations.at(0);

	for (int i = 0; i < animations.size(); i++)
	{
		if (animations.at(i)->GetName() == animationName)
		{
			animation = animations.at(i);
			break;
		}
	}

	if (time < animation->GetStartTime())
	{
		time = animation->GetStartTime();
	}

	if (time > animation->GetEndTime())
	{
		time = animation->GetStartTime();
	}

	for (int i = 0; i < bones.size(); i++)
	{
		bones.at(i)->UpdateKeyframeTransform(animation, time);
	}

	time += timePeriod;

	return UpdateBoneMatsVector();
}

AnimatedModel::~AnimatedModel()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes.at(i);
		meshes.at(i) = nullptr;
	}

	meshes.clear();

	for (int i = 0; i < animations.size(); i++)
	{
		delete animations.at(i);
		animations.at(i) = nullptr;
	}

	animations.clear();

	for (int i = 0; i < bones.size(); i++)
	{
		delete bones.at(i);
		bones.at(i) = nullptr;
	}

	bones.clear();

	for (int i = 0; i < nodes.size(); i++)
	{
		delete nodes.at(i);
		nodes.at(i) = nullptr;
	}

	nodes.clear();
}
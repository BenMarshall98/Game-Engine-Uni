#define GLM_ENABLE_EXPERIMENTAL

#include "AnimatedModel.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

void AnimatedModel::Render(Shader * const shader)
{
	shader->UseShader();

	if (boneMats.size() == 0)
	{
		for (int i = 0; i < 100; i++)
		{
			boneMats.push_back(glm::mat4(1.0));
		}
	}

	for (int i = 0; i < boneMats.size(); i++)
	{
		int boneLocation = glGetUniformLocation(shader->ShaderID(), ("Bones[" + std::to_string(i) + ']').c_str());

		glUniformMatrix4fv(boneLocation, 1, GL_TRUE, glm::value_ptr(boneMats[i]));
	}

	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i)->Render();
	}
}

Bone * AnimatedModel::FindBone(const std::string name)
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

void AnimatedModel::UpdateBoneMatsVector()
{
	if (bones.size() == 0)
	{
		return;
	}

	boneMats.clear();

	for (int i = 0; i < 100; i++)
	{
		if (i > bones.size() -1)
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
}

void AnimatedModel::Update()
{
	UpdateBoneMatsVector();

	Animation * const animation = GetFirstAnimation();

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
		bones.at(i)->UpdateKeyframeTransform(time);
	}

	time += (1.0 / 60.0);
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
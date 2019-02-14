#define GLM_ENABLE_EXPERIMENTAL

#include "AnimatedModel.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

void AnimatedModel::Render(Shader * shader)
{
	shader->UseShader();

	if (boneMats.size() == 0)
	{
		for (int i = 0; i < 100; i++)
		{
			boneMats.push_back(mat4(1.0));
		}
	}

	for (int i = 0; i < boneMats.size(); i++)
	{
		string bone = "Bones[" + to_string(i) + ']';
		int boneLocation = glGetUniformLocation(shader->ShaderID(), bone.c_str());

		if (boneLocation != -1)
		{
			int j = 0;
		}
		glUniformMatrix4fv(boneLocation, 1, GL_TRUE, value_ptr(boneMats[i]));
	}

	for (int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i)->Render();
	}
}

aiMatrix4x4 AnimatedModel::GLMMat4ToAi(mat4 mat)
{
	return aiMatrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
		mat[1][0], mat[1][1], mat[1][2], mat[1][3],
		mat[2][0], mat[2][1], mat[2][2], mat[2][3],
		mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

mat4 AnimatedModel::AiToGLMMat4(aiMatrix4x4& mat)
{
	mat4 tmp;
	tmp[0][0] = mat.a1;
	tmp[1][0] = mat.b1;
	tmp[2][0] = mat.c1;
	tmp[3][0] = mat.d1;

	tmp[0][1] = mat.a2;
	tmp[1][1] = mat.b2;
	tmp[2][1] = mat.c2;
	tmp[3][1] = mat.d2;

	tmp[0][2] = mat.a3;
	tmp[1][2] = mat.b3;
	tmp[2][2] = mat.c3;
	tmp[3][2] = mat.d3;

	tmp[0][3] = mat.a4;
	tmp[1][3] = mat.b4;
	tmp[2][3] = mat.c4;
	tmp[3][3] = mat.d4;

	return tmp;
}

Bone * AnimatedModel::FindBone(string name)
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
			boneMats.push_back(mat4(1.0));
		}
		else
		{
			mat4 transform = bones.at(i)->GetNode()->GetTransform();
			mat4 tmp = mat4(1.0);
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

	Animation * animation = GetFirstAnimation();

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
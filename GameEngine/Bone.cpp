#define GLM_ENABLE_EXPERIMENTAL

#include "Bone.h"
#include "AnimatedModel.h"
#include "glm/gtx/transform.hpp"

Bone::~Bone()
{

}

unsigned int Bone::FindPosition(float time)
{
	for (unsigned int i = 0; i < animNode->GetPositionKeys().size() - 1; i++)
	{
		if (time < animNode->GetPositionKeys()[i + 1]->GetTime())
		{
			return i;
		}
	}
	return 0;
}

unsigned int Bone::FindRotation(float time)
{
	for (unsigned int i = 0; i < animNode->GetRotationKeys().size() - 1; i++)
	{
		if (time < (float)animNode->GetRotationKeys()[i + 1]->GetTime())
		{
			return i;
		}
	}

	return 0;
}

vec3 Bone::CalcInterpolatedPosition(float time)
{
	if (animNode->GetPositionKeys().size() == 1)
	{
		vec3 pos = animNode->GetPositionKeys()[0]->GetValue();
		return pos;
	}

	unsigned int PositionIndex = FindPosition(time);
	unsigned int NextPositionIndex = (PositionIndex + 1);

	float pos1Time = (float)animNode->GetPositionKeys()[PositionIndex]->GetTime();
	float pos2Time = (float)animNode->GetPositionKeys()[NextPositionIndex]->GetTime();

	float DeltaTime = pos2Time - pos1Time;
	float Factor = (time - pos1Time) / DeltaTime;

	vec3 p1 = animNode->GetPositionKeys()[PositionIndex]->GetValue();
	vec3 p2 = animNode->GetPositionKeys()[NextPositionIndex]->GetValue();

	vec3 pos = mix(p1, p2, Factor);

	return pos;
}

quat Bone::CalcInterpolatedRotation(float time)
{
	if (animNode->GetRotationKeys().size() == 1)
	{
		quat rot = animNode->GetRotationKeys()[0]->GetValue();
		return rot;
	}

	unsigned int RotationIndex = FindRotation(time);
	unsigned int NextRotationIndex = (RotationIndex + 1);

	float rot1Time = (float)animNode->GetRotationKeys()[RotationIndex]->GetTime();
	float rot2Time = (float)animNode->GetRotationKeys()[NextRotationIndex]->GetTime();

	float DeltaTime = rot2Time - rot1Time;
	float Factor = (time - rot1Time) / DeltaTime;

	quat r1 = animNode->GetRotationKeys()[RotationIndex]->GetValue();
	quat r2 = animNode->GetRotationKeys()[NextRotationIndex]->GetValue();

	quat rot = slerp(r1, r2, Factor);

	return rot;
}

void Bone::UpdateKeyframeTransform(float time)
{
	if (animNode == nullptr)
	{
		return;
	}

	vec3 pos = CalcInterpolatedPosition(time);
	quat rot = CalcInterpolatedRotation(time);
	vec3 sca(1.0);

	mat4 mat(1.0);
	mat *= translate(pos);
	mat *= mat4_cast(rot);
	mat *= scale(sca);

	mat = transpose(mat);

	node->mTransformation = AnimatedModel::GLMMat4ToAi(mat);
}

mat4 Bone::GetParentTransforms()
{
	Bone * parent = parentBone;

	vector<mat4> mats;

	while (parent != nullptr)
	{
		mat4 tmp = AnimatedModel::AiToGLMMat4(parent->node->mTransformation);
		mats.push_back(tmp);

		parent = parent->parentBone;
	}

	mat4 concatenated_transforms = mat4(1.0);

	for (int i = mats.size() - 1; i >= 0; i--)
	{
		concatenated_transforms *= mats.at(i);
	}

	return concatenated_transforms;
}
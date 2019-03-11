#define GLM_ENABLE_EXPERIMENTAL

#include "Bone.h"
#include "AnimatedModel.h"
#include "glm/gtx/transform.hpp"

Bone::~Bone()
{

}

unsigned int Bone::FindPosition(const float time)
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

unsigned int Bone::FindRotation(const float time)
{
	for (unsigned int i = 0; i < animNode->GetRotationKeys().size() - 1; i++)
	{
		if (time < animNode->GetRotationKeys()[i + 1]->GetTime())
		{
			return i;
		}
	}

	return 0;
}

glm::vec3 Bone::CalcInterpolatedPosition(const float time)
{
	if (animNode->GetPositionKeys().size() == 1)
	{
		const glm::vec3 pos = animNode->GetPositionKeys()[0]->GetValue();
		return pos;
	}

	const unsigned int PositionIndex = FindPosition(time);
	const unsigned int NextPositionIndex = (PositionIndex + 1);

	const float pos1Time = animNode->GetPositionKeys()[PositionIndex]->GetTime();
	const float pos2Time = animNode->GetPositionKeys()[NextPositionIndex]->GetTime();

	const float DeltaTime = pos2Time - pos1Time;
	const float Factor = (time - pos1Time) / DeltaTime;

	const glm::vec3 p1 = animNode->GetPositionKeys()[PositionIndex]->GetValue();
	const glm::vec3 p2 = animNode->GetPositionKeys()[NextPositionIndex]->GetValue();

	const glm::vec3 pos = mix(p1, p2, Factor);

	return pos;
}

glm::quat Bone::CalcInterpolatedRotation(const float time)
{
	if (animNode->GetRotationKeys().size() == 1)
	{
		const glm::quat rot = animNode->GetRotationKeys()[0]->GetValue();
		return rot;
	}

	const unsigned int RotationIndex = FindRotation(time);
	const unsigned int NextRotationIndex = (RotationIndex + 1);

	const float rot1Time = animNode->GetRotationKeys()[RotationIndex]->GetTime();
	const float rot2Time = animNode->GetRotationKeys()[NextRotationIndex]->GetTime();

	const float DeltaTime = rot2Time - rot1Time;
	const float Factor = (time - rot1Time) / DeltaTime;

	const glm::quat r1 = animNode->GetRotationKeys()[RotationIndex]->GetValue();
	const glm::quat r2 = animNode->GetRotationKeys()[NextRotationIndex]->GetValue();

	const glm::quat rot = slerp(r1, r2, Factor);

	return rot;
}

void Bone::UpdateKeyframeTransform(const float time)
{
	if (animNode == nullptr)
	{
		return;
	}

	const glm::vec3 pos = CalcInterpolatedPosition(time);
	const glm::quat rot = CalcInterpolatedRotation(time);
	const glm::vec3 sca(1.0);

	glm::mat4 mat(1.0);
	mat *= translate(pos);
	mat *= mat4_cast(rot);
	mat *= scale(sca);

	mat = transpose(mat);

	node->SetTransform(mat);
}

glm::mat4 Bone::GetParentTransforms()
{
	Bone * parent = parentBone;

	std::vector<glm::mat4> mats;

	while (parent != nullptr)
	{
		const glm::mat4 tmp = parent->GetNode()->GetTransform();
		mats.push_back(tmp);

		parent = parent->parentBone;
	}

	glm::mat4 concatenated_transforms = glm::mat4(1.0);

	for (int i = mats.size() - 1; i >= 0; i--)
	{
		concatenated_transforms *= mats.at(i);
	}

	return concatenated_transforms;
}
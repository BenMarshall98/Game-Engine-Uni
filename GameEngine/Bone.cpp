#define GLM_ENABLE_EXPERIMENTAL

#include "Bone.h"
#include "AnimatedModel.h"
#include "glm/gtx/transform.hpp"

Bone::~Bone()
{

}

unsigned int Bone::FindPosition(AnimNode * animNode, const float time)
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

unsigned int Bone::FindRotation(AnimNode * animNode, const float time)
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

unsigned int Bone::FindScale(AnimNode * animNode, const float time)
{
	for (unsigned int i = 0; i < animNode->GetScaleKeys().size() - 1; i++)
	{
		if (time < animNode->GetScaleKeys()[i + 1]->GetTime())
		{
			return i;
		}
	}

	return 0;
}

glm::vec3 Bone::CalcInterpolatedPosition(AnimNode * animNode, const float time)
{
	if (animNode->GetPositionKeys().size() == 1)
	{
		const glm::vec3 pos = animNode->GetPositionKeys()[0]->GetValue();
		return pos;
	}

	const unsigned int PositionIndex = FindPosition(animNode, time);
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

glm::vec3 Bone::CalcInterpolatedScale(AnimNode * animNode, const float time)
{
	if (animNode->GetScaleKeys().size() == 1)
	{
		const glm::vec3 sca = animNode->GetScaleKeys()[0]->GetValue();
		return sca;
	}
	
	const unsigned int ScaleIndex = FindScale(animNode, time);
	const unsigned int NextScaleIndex = (ScaleIndex + 1);

	const float sca1Time = animNode->GetScaleKeys()[ScaleIndex]->GetTime();
	const float sca2Time = animNode->GetScaleKeys()[NextScaleIndex]->GetTime();

	const float DeltaTime = sca2Time - sca1Time;
	const float Factor = (time - sca1Time) / DeltaTime;

	const glm::vec3 s1 = animNode->GetPositionKeys()[ScaleIndex]->GetValue();
	const glm::vec3 s2 = animNode->GetPositionKeys()[NextScaleIndex]->GetValue();

	const glm::vec3 sca = mix(s1, s2, Factor);

	return sca;
}

glm::quat Bone::CalcInterpolatedRotation(AnimNode * animNode, const float time)
{
	if (animNode->GetRotationKeys().size() == 1)
	{
		const glm::quat rot = animNode->GetRotationKeys()[0]->GetValue();
		return rot;
	}

	const unsigned int RotationIndex = FindRotation(animNode, time);
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

void Bone::UpdateKeyframeTransform(Animation * animation, const float time)
{
	std::map<Animation *, AnimNode *>::iterator it = animNodes.find(animation);

	if (it == animNodes.end())
	{
		return;
	}

	if (it->second == nullptr)
	{
		return;
	}

	const glm::vec3 pos = CalcInterpolatedPosition(it->second, time);
	const glm::quat rot = CalcInterpolatedRotation(it->second, time);
	//const glm::vec3 sca = CalcInterpolatedScale(time);

	glm::mat4 mat(1.0);
	//mat *= scale(sca);
	mat *= translate(pos);
	mat *= mat4_cast(rot);

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
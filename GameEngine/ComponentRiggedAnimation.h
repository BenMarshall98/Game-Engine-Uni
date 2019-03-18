#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "iComponent.h"

enum class RiggedAnimationState
{
	PLAY,
	PAUSE,
	STOP,
	NONE
};

class ComponentRiggedAnimation : public iComponent
{
private:
	std::string animation;
	float time;
	RiggedAnimationState animationState;
	std::vector<glm::mat4> boneMats;

public:
	ComponentRiggedAnimation(const std::string & pAnimation, const RiggedAnimationState pAnimationState) : animation(pAnimation), animationState(pAnimationState) {}
	~ComponentRiggedAnimation();

	ComponentType GetComponentName() override;
	void RenderSwap() override;

	inline float GetTime()
	{
		return time;
	}

	inline void SetTime(float pTime)
	{
		time = pTime;
	}

	inline std::string GetAnimation()
	{
		return animation;
	}

	inline void SetAnimation(std::string pAnimation)
	{
		animation = pAnimation;
	}

	inline std::vector<glm::mat4> GetBoneMats()
	{
		return boneMats;
	}

	inline void SetBoneMats(std::vector<glm::mat4> pBoneMats)
	{
		boneMats = pBoneMats;
	}
};


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
	ComponentRiggedAnimation(const std::string & pAnimation, const RiggedAnimationState pAnimationState) :
		iComponent(ComponentType::COMPONENT_RIGGED_ANIMATION), animation(pAnimation), animationState(pAnimationState), time(0) {}
	~ComponentRiggedAnimation();

	void RenderSwap() override;

	//Gets the current animation time
	inline float GetTime()
	{
		return time;
	}

	//Sets the current animation time
	inline void SetTime(const float pTime)
	{
		time = pTime;
	}

	//Gets the name of the animation
	inline std::string & GetAnimation()
	{
		return animation;
	}

	//Sets the name of the animation
	inline void SetAnimation(const std::string & pAnimation)
	{
		animation = pAnimation;
	}

	//Gets the bone matrixs for animation
	inline std::vector<glm::mat4> & GetBoneMats()
	{
		return boneMats;
	}

	//Sets the bone matrixs for animation
	inline void SetBoneMats(const std::vector<glm::mat4> & pBoneMats)
	{
		boneMats = pBoneMats;
	}
};


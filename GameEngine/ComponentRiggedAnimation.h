#pragma once
#include <string>
#include "iComponent.h"

enum class RiggedAnimationState
{
	PLAY,
	PAUSE,
	STOP
};

class ComponentRiggedAnimation : public iComponent
{
private:
	std::string nextAnimation;
	RiggedAnimationState animationState;

public:
	ComponentRiggedAnimation(const std::string & pAnimation, const RiggedAnimationState pAnimationState) : nextAnimation(pAnimation), animationState(pAnimationState) {}
	~ComponentRiggedAnimation();

	ComponentType GetComponentName() override;
	void RenderSwap() override;
};


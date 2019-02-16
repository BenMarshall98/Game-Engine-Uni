#pragma once
#include <string>
#include "iComponent.h"

using namespace std;

enum class RiggedAnimationState
{
	PLAY,
	PAUSE,
	STOP
};

class ComponentRiggedAnimation : public iComponent
{
private:
	string nextAnimation;
	RiggedAnimationState animationState;

public:
	ComponentRiggedAnimation(const string & pAnimation, const RiggedAnimationState pAnimationState) : nextAnimation(pAnimation), animationState(pAnimationState) {}
	~ComponentRiggedAnimation();

	ComponentType GetComponentName() override;
	void RenderSwap() override;
};


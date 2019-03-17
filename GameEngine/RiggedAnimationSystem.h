#pragma once

#include "iSystem.h"
#include "EntityManager.h"
#include "Entity.h"
#include "AnimatedModel.h"
#include "ComponentRiggedAnimation.h"
#include <vector>

class RiggedAnimationSystem : public iSystem
{
private:
	static void Animate(AnimatedModel * animatedModel, ComponentRiggedAnimation * riggedAnimation);
	
public:
	RiggedAnimationSystem();
	~RiggedAnimationSystem();

	RiggedAnimationSystem& operator=(const RiggedAnimationSystem&) = delete;
	RiggedAnimationSystem(RiggedAnimationSystem&) = delete;

	void Action(void) override;
};


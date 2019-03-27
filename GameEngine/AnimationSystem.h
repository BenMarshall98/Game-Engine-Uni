#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "iSystem.h"
#include <vector>

class AnimationSystem : public iSystem
{
private:
	static void Animation(const std::string & function, Entity * entity, float timePeriod);

public:
	AnimationSystem();

	AnimationSystem& operator=(const AnimationSystem&) = delete;
	AnimationSystem(AnimationSystem&) = delete;

	void Action(float timePeriod) override;

	~AnimationSystem();
};


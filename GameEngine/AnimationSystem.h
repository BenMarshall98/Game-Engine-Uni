#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "iSystem.h"
#include <vector>

class AnimationSystem : public iSystem
{
private:
	std::vector<Entity *> EntityList;
	EntityManager * entityManager;

	const ComponentType componentTypes[1] = {
		ComponentType::COMPONENT_ANIMATION
	};

	static void Animation(const std::string & function, Entity * entity);

public:
	AnimationSystem();

	AnimationSystem& operator=(const AnimationSystem&) = delete;
	AnimationSystem(AnimationSystem&) = delete;

	void Action(void) override;
	void RemoveEntity(Entity * pEntity) override;
	void AddEntity(Entity * pEntity) override;

	~AnimationSystem();
};


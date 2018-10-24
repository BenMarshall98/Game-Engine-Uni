#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "iSystem.h"
#include <vector>

class RenderSystem : public iSystem
{
private:
	vector<Entity *> EntityList;
public:
	RenderSystem(EntityManager & entityManager);
	void OnAction(void);
	~RenderSystem();
};


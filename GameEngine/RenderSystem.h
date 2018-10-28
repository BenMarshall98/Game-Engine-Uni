#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "iSystem.h"
#include "iModel.h"
#include "glm/glm.hpp"
#include "Shader.h"
#include <vector>

using namespace glm;

class RenderSystem : public iSystem
{
private:
	vector<Entity *> EntityList;
	EntityManager & entityManager;
public:
	RenderSystem(EntityManager & entityManager);
	void Action(void);
	void Render(Shader * shader, iModel * model, vec3 * position);
	~RenderSystem();
};


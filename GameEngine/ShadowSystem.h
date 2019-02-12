#pragma once
#include "iSystem.h"

#include "Entity.h"
#include "EntityManager.h"
#include "glm/glm.hpp"
#include "Shader.h"
#include "iModel.h"
#include "LightManager.h"
#include <vector>

using namespace glm;

class ShadowSystem : public iSystem
{
private:
	vector<Entity *> EntityList;
	EntityManager * entityManager;
	vec3 mTopLeftCoord;
	vec3 mBottomRightCoord;

	static void RenderDirectional(iModel * model, vec3 & position, quat & direction, Directional * directional, Shader * shadowShader);

	static void RenderPoint(iModel * model, vec3 & position, quat & direction, Shader * shadowShader);

public:
	ShadowSystem(vec3 & topLeftFrontCoord, vec3 & bottomRightBackCoord);

	ShadowSystem& operator=(const ShadowSystem&) = delete;
	ShadowSystem(ShadowSystem&) = delete;

	void Action(void) override;
	void RemoveEntity(Entity * pEntity) override;

	~ShadowSystem();
};


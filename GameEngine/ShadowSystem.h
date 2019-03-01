#pragma once
#include "iSystem.h"

#include "Entity.h"
#include "EntityManager.h"
#include "glm/glm.hpp"
#include "Shader.h"
#include "iModel.h"
#include "LightManager.h"
#include <vector>

class ShadowSystem : public iSystem
{
private:
	glm::vec3 mTopLeftCoord;
	glm::vec3 mBottomRightCoord;

	std::vector<Entity *> EntityList;
	std::vector<std::string> views;
	EntityManager * entityManager;

	static void RenderDirectional(iModel * model, glm::vec3 & position, glm::quat & direction, Directional * directional, Shader * shadowShader);

	static void RenderPoint(iModel * model, glm::vec3 & position, glm::quat & direction, Shader * shadowShader);

public:
	ShadowSystem(glm::vec3 & topLeftFrontCoord, glm::vec3 & bottomRightBackCoord);

	ShadowSystem& operator=(const ShadowSystem&) = delete;
	ShadowSystem(ShadowSystem&) = delete;

	void Action(void) override;
	void RemoveEntity(Entity * pEntity) override;

	~ShadowSystem();
};


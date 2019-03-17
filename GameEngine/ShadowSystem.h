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

	std::vector<std::string> views;

	static void RenderDirectional(iModel * const model, const glm::vec3 & position, const glm::quat & direction, Directional * const directional, Shader * const shadowShader);

	static void RenderPoint(iModel * const model, const glm::vec3 & position, const glm::quat & direction, Shader * const shadowShader);

public:
	ShadowSystem(glm::vec3 & topLeftFrontCoord, glm::vec3 & bottomRightBackCoord);

	ShadowSystem& operator=(const ShadowSystem&) = delete;
	ShadowSystem(ShadowSystem&) = delete;

	void Action(void) override;

	~ShadowSystem();
};


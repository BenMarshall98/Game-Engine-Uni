#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "iSystem.h"
#include "ComponentRiggedAnimation.h"
#include "iModel.h"
#include "glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Projection.h"
#include <vector>

class RenderSystem : public iSystem
{
private:
	Camera * camera;
	Projection * projection;
	bool updateFirst;

	static void Render(Shader * const shader, iModel * const model, const glm::vec3 & position, const glm::quat & direction, Texture * const texture, Texture * const normal, const glm::mat4 & perspectiveMatrix, const glm::mat4 & viewMatrix, const glm::vec3 & viewPos, ComponentRiggedAnimation * riggedAnimation, bool & updateFirst);

public:

	RenderSystem();

	RenderSystem& operator=(const RenderSystem&) = delete;
	RenderSystem(RenderSystem&) = delete;

	void Action(float timePeriod) override;
	
	~RenderSystem();
};


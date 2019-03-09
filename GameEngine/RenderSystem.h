#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "iSystem.h"
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
	std::vector<Entity *> EntityList;
	EntityManager * entityManager;
	Camera * camera;
	Projection * projection;
	bool updateFirst;

	const ComponentType componentTypes[5] = {
		ComponentType::COMPONENT_MODEL,
		ComponentType::COMPONENT_SHADER,
		ComponentType::COMPONENT_POSITION,
		ComponentType::COMPONENT_TEXTURE,
		ComponentType::COMPONENT_DIRECTION
	};

	static void Render(Shader * shader, iModel * model, glm::vec3 & position, glm::quat & direction, const Texture * texture, const Texture * normal, glm::mat4 & perspectiveMatrix, glm::mat4 & viewMatrix, glm::vec3 & viewPos, bool & updateFirst);

public:

	RenderSystem();

	RenderSystem& operator=(const RenderSystem&) = delete;
	RenderSystem(RenderSystem&) = delete;

	void Action(void) override;
	void RemoveEntity(Entity * pEntity) override;
	void AddEntity(Entity * pEntity) override;
	
	~RenderSystem();
};


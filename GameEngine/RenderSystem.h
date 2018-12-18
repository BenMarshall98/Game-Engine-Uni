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

using namespace glm;

class RenderSystem : public iSystem
{
private:
	vector<Entity *> EntityList;
	EntityManager * entityManager;
	Camera * camera;
	Projection * projection;
	bool updateFirst;

	void Render(Shader * shader, iModel * model, vec3 position, quat direction, Texture * texture, Texture * normal, mat4 perspectiveMatrix, mat4 viewMatrix, vec3 viewPos);

public:

	RenderSystem(Camera * pCamera, Projection * pProjection);
	void Action(void) override;
	void RemoveEntity(Entity * pEntity) override;
	
	~RenderSystem();
};


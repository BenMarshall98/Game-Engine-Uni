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
	EntityManager & entityManager;
	Camera * camera;
	Projection * projection;

	void Render(Shader * shader, iModel * model, vec3 position, Texture * texture, mat4 perspectiveMatrix, mat4 viewMatrix);

public:

	RenderSystem(EntityManager & entityManager, Camera * pCamera, Projection * pProjection);
	void Action(void);
	
	~RenderSystem();
};


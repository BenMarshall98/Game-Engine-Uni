#define GLM_ENABLE_EXPERIMENTAL

#include "RenderSystem.h"
#include "ComponentModel.h"
#include "ComponentPosition.h"
#include "ComponentShader.h"
#include "ComponentTexture.h"
#include "ComponentNormalTexture.h"
#include "ComponentDirection.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "GLFWWindow.h"
#include "LightManager.h"
#include <string>
#include <algorithm>

RenderSystem::RenderSystem(Camera * pCamera, Projection * pProjection) : camera(pCamera), projection(pProjection), updateFirst(true)
{
	entityManager = EntityManager::Instance();
	ComponentType componentTypes[] = { COMPONENT_MODEL, COMPONENT_SHADER, COMPONENT_POSITION, COMPONENT_TEXTURE, COMPONENT_DIRECTION };
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, size(componentTypes));
}

void RenderSystem::RemoveEntity(Entity * pEntity)
{
	vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void RenderSystem::Action(void)
{
	mat4 perspectiveMatrix = projection->GetProjection();
	mat4 viewMatrix = camera->GetViewMatrix();
	vec3 viewPos = camera->GetPosition();

	LightManager * lightManager = LightManager::Instance();
	lightManager->Update(camera->GetPosition());
	updateFirst = true;

	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentShader = entityManager->GetComponentOfEntity(EntityList[i], COMPONENT_SHADER);
		iComponent * componentModel = entityManager->GetComponentOfEntity(EntityList[i], COMPONENT_MODEL);
		iComponent * componentPosition = entityManager->GetComponentOfEntity(EntityList[i], COMPONENT_POSITION);
		iComponent * componentTexture = entityManager->GetComponentOfEntity(EntityList[i], COMPONENT_TEXTURE);
		iComponent * componentDirection = entityManager->GetComponentOfEntity(EntityList[i], COMPONENT_DIRECTION);

		Shader * shader = dynamic_cast<ComponentShader *>(componentShader)->GetShader();
		iModel * model = dynamic_cast<ComponentModel *>(componentModel)->GetModel();
		vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetPosition();
		Texture * texture = dynamic_cast<ComponentTexture *>(componentTexture)->GetTexture();
		quat direction = dynamic_cast<ComponentDirection *>(componentDirection)->GetDirection();

		iComponent * componentNormal = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_NORMAL);

		Texture * normal = (componentNormal == nullptr) ? nullptr : dynamic_cast<ComponentNormalTexture *>(componentNormal)->GetTexture();

		Render(shader, model, position, direction, texture, normal, perspectiveMatrix, viewMatrix, viewPos);
	}
}

void RenderSystem::Render(Shader * shader, iModel * model, vec3 position, quat direction, Texture * texture, Texture * normal, mat4 perspectiveMatrix, mat4 viewMatrix, vec3 viewPos)
{
	static int lastShader = -1;

	shader->UseShader();

	if (lastShader != shader->ShaderID() || updateFirst)
	{
		LightManager * lightManager = LightManager::Instance();
		lightManager->Render(shader->ShaderID());
		lastShader = shader->ShaderID();
		updateFirst = false;
	}

	mat4 modelMatrix(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix *= toMat4(direction);

	int modelLocation = glGetUniformLocation(shader->ShaderID(), "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelMatrix));

	int perspectiveLocation = glGetUniformLocation(shader->ShaderID(), "perspective");
	glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, value_ptr(perspectiveMatrix));

	int viewLocation = glGetUniformLocation(shader->ShaderID(), "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(viewMatrix));

	int textureLocation = glGetUniformLocation(shader->ShaderID(), "texture_map");
	glUniform1i(textureLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->TextureID());

	if (normal != nullptr)
	{
		int normalLocation = glGetUniformLocation(shader->ShaderID(), "normalTexture");
		glUniform1i(normalLocation, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal->TextureID());
	}

	int viewPosLocation = glGetUniformLocation(shader->ShaderID(), "viewPos");
	glUniform3fv(viewPosLocation, 1, value_ptr(viewPos));

	model->Render(shader);
	glUseProgram(0);
}

RenderSystem::~RenderSystem()
{
}

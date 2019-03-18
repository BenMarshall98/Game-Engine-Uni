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
#include "CameraManager.h"
#include <string>
#include <algorithm>

RenderSystem::RenderSystem() : iSystem(std::vector<ComponentType>{
	ComponentType::COMPONENT_MODEL,
	ComponentType::COMPONENT_SHADER,
	ComponentType::COMPONENT_POSITION,
	ComponentType::COMPONENT_TEXTURE,
	ComponentType::COMPONENT_DIRECTION
}), updateFirst(true), camera(CameraManager::Instance()->GetCamera()), projection(CameraManager::Instance()->GetProjection())
{
}

void RenderSystem::Action(void)
{
	EntityManager * entityManager = EntityManager::Instance();
	glViewport(0, 0, GLFWWindow::GetWidth(), GLFWWindow::GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 perspectiveMatrix = projection->GetProjection();
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glm::vec3 viewPos = camera->GetPosition();

	updateFirst = true;

	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * const componentShader = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_SHADER);
		iComponent * const componentModel = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_MODEL);
		iComponent * const componentPosition = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_POSITION);
		iComponent * const componentTexture = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_TEXTURE);
		iComponent * const componentDirection = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_DIRECTION);

		Shader * const shader = dynamic_cast<ComponentShader *>(componentShader)->GetRenderShader();
		iModel * const model = dynamic_cast<ComponentModel *>(componentModel)->GetRenderModel();
		glm::vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetRenderPosition();
		Texture * const texture = dynamic_cast<ComponentTexture *>(componentTexture)->GetRenderTexture();
		glm::quat direction = dynamic_cast<ComponentDirection *>(componentDirection)->GetRenderDirection();

		iComponent * const componentNormal = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_NORMAL_TEXTURE);
		const Texture * const normal = (componentNormal == nullptr) ? nullptr : dynamic_cast<ComponentNormalTexture *>(componentNormal)->GetRenderTexture();

		iComponent * const componentRiggedAnimation = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_RIGGED_ANIMATION);
		ComponentRiggedAnimation * riggedAnimation = (componentRiggedAnimation == nullptr) ? nullptr : dynamic_cast<ComponentRiggedAnimation *>(componentRiggedAnimation);

		if (length(position - viewPos) < (projection->GetFar() * 1.1f))
		{
			Render(shader, model, position, direction, texture, normal, perspectiveMatrix, viewMatrix, viewPos, riggedAnimation, updateFirst);
		}
	}
}

void RenderSystem::Render(Shader * const shader, iModel * const model, const glm::vec3 & position, const glm::quat & direction, const Texture * const texture, const Texture * const normal, glm::mat4 & perspectiveMatrix, glm::mat4 & viewMatrix, glm::vec3 & viewPos, ComponentRiggedAnimation * riggedAnimation, bool & updateFirst)
{
	static int lastShader = -1;

	shader->UseShader();

	static int modelLocation;
	static int perspectiveLocation;
	static int viewLocation;
	static int textureLocation;
	static int normalLocation;
	static int viewPosLocation;

	if (lastShader != shader->ShaderID() || updateFirst)
	{
		LightManager * const lightManager = LightManager::Instance();
		lightManager->Render(shader->ShaderID());
		lastShader = shader->ShaderID();
		updateFirst = false;
		modelLocation = glGetUniformLocation(shader->ShaderID(), "model");
		perspectiveLocation = glGetUniformLocation(shader->ShaderID(), "perspective");
		viewLocation = glGetUniformLocation(shader->ShaderID(), "view");
		textureLocation = glGetUniformLocation(shader->ShaderID(), "texture_map");
		normalLocation = glGetUniformLocation(shader->ShaderID(), "normalTexture");
		viewPosLocation = glGetUniformLocation(shader->ShaderID(), "viewPos");
	}

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix *= toMat4(direction);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelMatrix));

	glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, value_ptr(perspectiveMatrix));

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(viewMatrix));

	glUniform1i(textureLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->TextureID());

	if (normal != nullptr)
	{
		glUniform1i(normalLocation, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal->TextureID());
	}

	glUniform3fv(viewPosLocation, 1, value_ptr(viewPos));

	if (riggedAnimation)
	{
		std::vector<glm::mat4> boneMats = riggedAnimation->GetBoneMats();
		model->Render(shader, boneMats);
	}
	else
	{
		model->Render(shader);
	}
	glUseProgram(0);
}

RenderSystem::~RenderSystem()
{
}
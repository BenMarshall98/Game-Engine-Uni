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
#include "RenderManager.h"

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
	EntityManager * const entityManager = EntityManager::Instance();
	RenderManager::Instance()->SetViewport(GLFWWindow::GetWidth(), GLFWWindow::GetHeight());
	RenderManager::Instance()->ClearColorBuffer();
	RenderManager::Instance()->ClearDepthBuffer();
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
		const glm::vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetRenderPosition();
		Texture * const texture = dynamic_cast<ComponentTexture *>(componentTexture)->GetRenderTexture();
		const glm::quat direction = dynamic_cast<ComponentDirection *>(componentDirection)->GetRenderDirection();

		iComponent * const componentNormal = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_NORMAL_TEXTURE);
		Texture * const normal = (componentNormal == nullptr) ? nullptr : dynamic_cast<ComponentNormalTexture *>(componentNormal)->GetRenderTexture();

		iComponent * const componentRiggedAnimation = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_RIGGED_ANIMATION);
		ComponentRiggedAnimation * const riggedAnimation = (componentRiggedAnimation == nullptr) ? nullptr : dynamic_cast<ComponentRiggedAnimation *>(componentRiggedAnimation);

		if (length(position - viewPos) < (projection->GetFar() * 1.1f))
		{
			Render(shader, model, position, direction, texture, normal, perspectiveMatrix, viewMatrix, viewPos, riggedAnimation, updateFirst);
		}
	}
}

void RenderSystem::Render(Shader * const shader, iModel * const model, const glm::vec3 & position, const glm::quat & direction, Texture * const texture, Texture * const normal, const glm::mat4 & perspectiveMatrix, const glm::mat4 & viewMatrix, const glm::vec3 & viewPos, ComponentRiggedAnimation * const  riggedAnimation, bool & updateFirst)
{
	static Shader * lastShader = nullptr;

	RenderManager * const renderManager = RenderManager::Instance();

	shader->UseShader();

	if (lastShader != shader || updateFirst)
	{
		LightManager * const lightManager = LightManager::Instance();
		lightManager->Render(shader);
		lastShader = shader;
		updateFirst = false;
	}

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix *= toMat4(direction);

	renderManager->SetUniformMatrix4fv(shader, "model", modelMatrix, false);
	renderManager->SetUniformMatrix4fv(shader, "perspective", perspectiveMatrix, false);
	renderManager->SetUniformMatrix4fv(shader, "view", viewMatrix, false);
	renderManager->UseTexture(shader, "texture_map", texture, 0);

	if (normal != nullptr)
	{
		renderManager->UseTexture(shader, "normalTexture", normal, 1);
	}

	renderManager->SetUniform3fv(shader, "viewPos", viewPos);

	if (riggedAnimation)
	{
		const std::vector<glm::mat4> boneMats = riggedAnimation->GetBoneMats();
		model->Render(shader, boneMats);
	}
	else
	{
		model->Render(shader);
	}

	renderManager->ClearShader();
}

RenderSystem::~RenderSystem()
{
}
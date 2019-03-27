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

void RenderSystem::Action(float timePeriod)
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

	RenderManager::Instance()->ClearShader();
}

void RenderSystem::Render(Shader * const shader, iModel * const model, const glm::vec3 & position, const glm::quat & direction, Texture * const texture, Texture * const normal, const glm::mat4 & perspectiveMatrix, const glm::mat4 & viewMatrix, const glm::vec3 & viewPos, ComponentRiggedAnimation * const  riggedAnimation, bool & updateFirst)
{
	static Shader * lastShader = nullptr;
	static UniformLocation * modelLocation = nullptr;
	static UniformLocation * perspectiveLocation = nullptr;
	static UniformLocation * viewLocation = nullptr;
	static UniformLocation * textureLocation = nullptr;
	static UniformLocation * normalLocation = nullptr;
	static UniformLocation * viewPosLocation = nullptr;

	RenderManager * const renderManager = RenderManager::Instance();

	if (lastShader != shader || updateFirst)
	{
		shader->UseShader();
		LightManager * const lightManager = LightManager::Instance();
		lightManager->Render(shader);
		lastShader = shader;
		updateFirst = false;

		modelLocation = renderManager->GetUniformLocation(shader, "model");
		perspectiveLocation = renderManager->GetUniformLocation(shader, "perspective");
		viewLocation = renderManager->GetUniformLocation(shader, "view");
		textureLocation = renderManager->GetUniformLocation(shader, "texture_map");
		normalLocation = renderManager->GetUniformLocation(shader, "normalTexture");
		viewPosLocation = renderManager->GetUniformLocation(shader, "viewPos");
	}

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix *= toMat4(direction);

	renderManager->SetUniformMatrix4fv(modelLocation, modelMatrix, false);
	renderManager->SetUniformMatrix4fv(perspectiveLocation, perspectiveMatrix, false);
	renderManager->SetUniformMatrix4fv(viewLocation, viewMatrix, false);
	renderManager->UseTexture(textureLocation, texture, 0);

	if (normal != nullptr)
	{
		renderManager->UseTexture(normalLocation, normal, 1);
	}

	renderManager->SetUniform3fv(viewPosLocation, viewPos);

	if (riggedAnimation)
	{
		const std::vector<glm::mat4> boneMats = riggedAnimation->GetBoneMats();
		model->Render(shader, boneMats);
	}
	else
	{
		model->Render(shader);
	}
}

RenderSystem::~RenderSystem()
{
}
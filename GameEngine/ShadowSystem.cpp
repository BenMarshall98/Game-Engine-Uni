#define GLM_ENABLE_EXPERIMENTAL

#include "ShadowSystem.h"
#include "ComponentDirection.h"
#include "ComponentPosition.h"
#include "ComponentModel.h"
#include "ComponentShadowShader.h"
#include "CameraManager.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include <algorithm>
#include "RenderManager.h"

ShadowSystem::ShadowSystem(glm::vec3 & topLeftCoord, glm::vec3 & bottomRightCoord) : iSystem(std::vector<ComponentType>{
	ComponentType::COMPONENT_MODEL,
	ComponentType::COMPONENT_POSITION,
	ComponentType::COMPONENT_DIRECTION,
	ComponentType::COMPONENT_SHADOW_SHADER
}),	mTopLeftCoord(topLeftCoord), mBottomRightCoord(bottomRightCoord)
{
	for (int i = 0; i < 6; i++)
	{
		const std::string view = "shadowView[" + std::to_string(i) + ']';
		views.push_back(view);
	}
}

void ShadowSystem::Action(void)
{
	RenderManager * renderManager = RenderManager::Instance();
	EntityManager * entityManager = EntityManager::Instance();
	std::vector<iModel *> models = std::vector<iModel *>();
	std::vector<glm::vec3> positions = std::vector<glm::vec3>();
	std::vector<glm::quat> directions = std::vector<glm::quat>();
	std::vector<Shader *> directionalShadowShaders = std::vector<Shader *>();
	std::vector<Shader *> pointShadowShaders = std::vector<Shader *>();
	Camera * camera = CameraManager::Instance()->GetCamera();
	Projection * projection = CameraManager::Instance()->GetProjection();

	int count = 0;
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * const componentModel = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_MODEL);
		iComponent * const componentPosition = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_POSITION);
		iComponent * const componentDirection = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_DIRECTION);
		iComponent * const componentShadowShader = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_SHADOW_SHADER);

		iModel * const model = dynamic_cast<ComponentModel *>(componentModel)->GetRenderModel();
		const glm::vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetRenderPosition();
		const glm::quat direction = dynamic_cast<ComponentDirection *>(componentDirection)->GetRenderDirection();
		Shader * const directionalShadowShader = dynamic_cast<ComponentShadowShader *>(componentShadowShader)->GetDirectionalShader();
		Shader * const pointShadowShader = dynamic_cast<ComponentShadowShader *>(componentShadowShader)->GetPointShader();

		if (length(position - camera->GetPosition()) < (projection->GetFar() * 1.1f))
		{
			models.push_back(model);
			positions.push_back(position);
			directions.push_back(direction);
			directionalShadowShaders.push_back(directionalShadowShader);
			pointShadowShaders.push_back(pointShadowShader);
		}
	}
	
	renderManager->CullFace("Front");

	Directional * const directional = LightManager::Instance()->GetDirectionalLight();

	directional->perspective = glm::ortho(mTopLeftCoord.x, mBottomRightCoord.x,
		mBottomRightCoord.y, mTopLeftCoord.y, mTopLeftCoord.z, mBottomRightCoord.z);
	
	directional->view = glm::lookAt(camera->GetPosition() + directional->direction, camera->GetPosition(), glm::vec3(0, 0, 1));

	renderManager->SetViewport(LightManager::shadowWidth, LightManager::shadowHeight);
	FrameBuffer * frameBuffer = LightManager::Instance()->GetDirectionalFramebuffer();
	renderManager->UseFrameBuffer(frameBuffer);
	renderManager->ClearDepthBuffer();

	for (int i = 0; i < models.size(); i++)
	{
		RenderDirectional(models.at(i), positions.at(i), directions.at(i), directional, directionalShadowShaders.at(i));
	}

	std::vector<PointLight *> pointLights = LightManager::Instance()->GetRenderPointLights();

	for (int i = 0; i < pointLights.size(); i++)
	{
		FrameBuffer * const frameBuffer = pointLights[i]->shadowTexture;

		renderManager->UseFrameBuffer(frameBuffer);
		renderManager->ClearDepthBuffer();

		float aspect = (float)LightManager::shadowWidth / (float)LightManager::shadowHeight;
		float farPlane = 25.0f;

		pointLights[i]->farPlane = farPlane;
		const glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 1.0f, farPlane);

		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(projection * glm::lookAt(pointLights[i]->location, pointLights[i]->location + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(projection * glm::lookAt(pointLights[i]->location, pointLights[i]->location + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(projection * glm::lookAt(pointLights[i]->location, pointLights[i]->location + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(projection * glm::lookAt(pointLights[i]->location, pointLights[i]->location + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(projection * glm::lookAt(pointLights[i]->location, pointLights[i]->location + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(projection * glm::lookAt(pointLights[i]->location, pointLights[i]->location + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		Shader * lastShader = nullptr;

		for (int j = 0; j < models.size(); j++)
		{
			if (lastShader != pointShadowShaders.at(j))
			{
				pointShadowShaders.at(j)->UseShader();

				renderManager->SetUniform3fv(pointShadowShaders.at(j), "lightPos", pointLights[i]->location);
				renderManager->SetUniform1f(pointShadowShaders.at(j), "farPlane", farPlane);

				for (int k = 0; k < shadowTransforms.size(); k++)
				{
					renderManager->SetUniformMatrix4fv(pointShadowShaders.at(j), views.at(k), shadowTransforms.at(k), false);
				}

				lastShader = pointShadowShaders.at(j);
			}

			RenderPoint(models.at(j), positions.at(j), directions.at(j), pointShadowShaders.at(j));
		}
	}

	std::vector<SpotLight *> spotLights = LightManager::Instance()->GetRenderSpotLights();

	for (int i = 0; i < spotLights.size(); i++)
	{
		FrameBuffer * const frameBuffer = spotLights[i]->shadowTexture;
		
		renderManager->UseFrameBuffer(frameBuffer);
		renderManager->ClearDepthBuffer();

		float aspect = (float)LightManager::shadowWidth / (float)LightManager::shadowHeight;
		float farPlane = 25.0f;

		spotLights[i]->farPlane = farPlane;
		const glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 1.0f, farPlane);

		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(projection * glm::lookAt(spotLights[i]->location, spotLights[i]->location + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(projection * glm::lookAt(spotLights[i]->location, spotLights[i]->location + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(projection * glm::lookAt(spotLights[i]->location, spotLights[i]->location + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(projection * glm::lookAt(spotLights[i]->location, spotLights[i]->location + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(projection * glm::lookAt(spotLights[i]->location, spotLights[i]->location + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(projection * glm::lookAt(spotLights[i]->location, spotLights[i]->location + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		Shader * lastShader = nullptr;

		for (int j = 0; j < models.size(); j++)
		{
			if (lastShader != pointShadowShaders.at(j))
			{
				pointShadowShaders.at(j)->UseShader();

				renderManager->SetUniform3fv(pointShadowShaders.at(j), "lightPos", spotLights[i]->location);
				renderManager->SetUniform1f(pointShadowShaders.at(j), "farPlane", farPlane);

				for (int k = 0; k < shadowTransforms.size(); k++)
				{
					const std::string view = views.at(k).c_str();
					renderManager->SetUniformMatrix4fv(pointShadowShaders.at(j), view, shadowTransforms.at(k), false);
				}

				lastShader = pointShadowShaders.at(j);
			}

			RenderPoint(models.at(j), positions.at(j), directions.at(j), pointShadowShaders.at(j));
		}
	}

	renderManager->ClearShader();
	renderManager->ClearFrameBuffer();
	renderManager->CullFace("Back");
}

void ShadowSystem::RenderDirectional(iModel * const model, const glm::vec3 & position, const glm::quat & direction, Directional * const directional, Shader * const shadowShader)
{
	RenderManager * renderManager = RenderManager::Instance();
	shadowShader->UseShader();

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix *= toMat4(direction);

	renderManager->SetUniformMatrix4fv(shadowShader, "model", modelMatrix, false);
	renderManager->SetUniformMatrix4fv(shadowShader, "perspective", directional->perspective, false);
	renderManager->SetUniformMatrix4fv(shadowShader, "view", directional->view, false);

	model->Render(shadowShader);
	
	renderManager->ClearShader();
}

void ShadowSystem::RenderPoint(iModel * const model, const glm::vec3 & position, const glm::quat & direction, Shader * const shadowShader)
{
	shadowShader->UseShader();

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix *= toMat4(direction);

	RenderManager::Instance()->SetUniformMatrix4fv(shadowShader, "model", modelMatrix, false);

	model->Render(shadowShader);
}

ShadowSystem::~ShadowSystem()
{
}

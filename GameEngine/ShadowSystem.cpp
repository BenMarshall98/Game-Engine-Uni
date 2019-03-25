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
	RenderManager * const renderManager = RenderManager::Instance();
	EntityManager * const entityManager = EntityManager::Instance();
	std::vector<iModel *> models = std::vector<iModel *>();
	std::vector<glm::vec3> positions = std::vector<glm::vec3>();
	std::vector<glm::quat> directions = std::vector<glm::quat>();
	std::vector<Shader *> directionalShadowShaders = std::vector<Shader *>();
	std::vector<Shader *> pointShadowShaders = std::vector<Shader *>();
	std::vector<glm::mat4> modelMatrics = std::vector<glm::mat4>();
	Camera * const camera = CameraManager::Instance()->GetCamera();
	Projection * const projection = CameraManager::Instance()->GetProjection();

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

			glm::mat4 modelMatrix(1.0f);
			modelMatrix = translate(modelMatrix, position);
			modelMatrix *= toMat4(direction);

			modelMatrics.push_back(modelMatrix);
		}
	}
	
	renderManager->CullFace("Front");

	Directional * const directional = LightManager::Instance()->GetDirectionalLight();

	directional->perspective = glm::ortho(mTopLeftCoord.x, mBottomRightCoord.x,
		mBottomRightCoord.y, mTopLeftCoord.y, mTopLeftCoord.z, mBottomRightCoord.z);
	
	directional->view = glm::lookAt(camera->GetPosition() + directional->direction, camera->GetPosition(), glm::vec3(0, 0, 1));

	renderManager->SetViewport(LightManager::shadowWidth, LightManager::shadowHeight);
	FrameBuffer * const frameBuffer = LightManager::Instance()->GetDirectionalFramebuffer();
	renderManager->UseFrameBuffer(frameBuffer);
	renderManager->ClearDepthBuffer();
	bool firstTime = true;

	for (int i = 0; i < models.size(); i++)
	{
		RenderDirectional(models.at(i), modelMatrics.at(i), directional, directionalShadowShaders.at(i), firstTime);
	}

	renderManager->ClearShader();

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
		bool firstTime = true;

		for (int j = 0; j < models.size(); j++)
		{
			if (lastShader != pointShadowShaders.at(j))
			{
				pointShadowShaders.at(j)->UseShader();

				UniformLocation * lightPosLocation = renderManager->GetUniformLocation(pointShadowShaders.at(j), "lightPos");
				renderManager->SetUniform3fv(lightPosLocation, pointLights[i]->location);

				UniformLocation * farPlaneLocation = renderManager->GetUniformLocation(pointShadowShaders.at(j), "farPlane");
				renderManager->SetUniform1f(farPlaneLocation, farPlane);

				delete lightPosLocation;
				delete farPlaneLocation;

				for (int k = 0; k < shadowTransforms.size(); k++)
				{
					UniformLocation * shadowViewLocation = renderManager->GetUniformLocation(pointShadowShaders.at(j), views.at(k));
					renderManager->SetUniformMatrix4fv(shadowViewLocation, shadowTransforms.at(k), false);

					delete shadowViewLocation;
				}

				lastShader = pointShadowShaders.at(j);
			}

			RenderPoint(models.at(j), modelMatrics.at(j), pointShadowShaders.at(j), firstTime);
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
		bool firstTime = true;

		for (int j = 0; j < models.size(); j++)
		{
			if (lastShader != pointShadowShaders.at(j))
			{
				pointShadowShaders.at(j)->UseShader();

				UniformLocation * lightPosLocation = renderManager->GetUniformLocation(pointShadowShaders.at(j), "lightPos");
				renderManager->SetUniform3fv(lightPosLocation, spotLights[i]->location);

				UniformLocation * farPlaneLocation = renderManager->GetUniformLocation(pointShadowShaders.at(j), "farPlane");
				renderManager->SetUniform1f(farPlaneLocation, farPlane);

				delete lightPosLocation;
				delete farPlaneLocation;

				for (int k = 0; k < shadowTransforms.size(); k++)
				{
					const std::string view = views.at(k).c_str();

					UniformLocation * shadowViewLocation = renderManager->GetUniformLocation(pointShadowShaders.at(j), view);
					renderManager->SetUniformMatrix4fv(shadowViewLocation, shadowTransforms.at(k), false);

					delete shadowViewLocation;
				}

				lastShader = pointShadowShaders.at(j);
			}

			RenderPoint(models.at(j), modelMatrics.at(j), pointShadowShaders.at(j), firstTime);
		}
	}

	renderManager->ClearShader();
	renderManager->ClearFrameBuffer();
	renderManager->CullFace("Back");
}

void ShadowSystem::RenderDirectional(iModel * const model, const glm::mat4 & modelMatrix, const Directional * const directional, Shader * const shadowShader, bool & firstTime)
{
	static Shader * lastShader = nullptr;
	static UniformLocation * modelLocation = nullptr;
	static UniformLocation * perspectiveLocation = nullptr;
	static UniformLocation * viewLocation = nullptr;

	RenderManager * const renderManager = RenderManager::Instance();

	if (lastShader != shadowShader || firstTime)
	{
		shadowShader->UseShader();
		modelLocation = renderManager->GetUniformLocation(shadowShader, "model");
		perspectiveLocation = renderManager->GetUniformLocation(shadowShader, "perspective");
		viewLocation = renderManager->GetUniformLocation(shadowShader, "view");
		lastShader = shadowShader;
		firstTime = false;
	}

	renderManager->SetUniformMatrix4fv(modelLocation, modelMatrix, false);
	renderManager->SetUniformMatrix4fv(perspectiveLocation, directional->perspective, false);
	renderManager->SetUniformMatrix4fv(viewLocation, directional->view, false);

	model->Render(shadowShader);
}

void ShadowSystem::RenderPoint(iModel * const model, const glm::mat4 & modelMatrix, Shader * const shadowShader, bool & firstTime)
{
	static Shader * lastShader = nullptr;
	static UniformLocation * modelLocation = nullptr;

	if (lastShader != shadowShader || firstTime)
	{
		shadowShader->UseShader();
		modelLocation = RenderManager::Instance()->GetUniformLocation(shadowShader, "model");
		lastShader = shadowShader;
		firstTime = false;
	}

	RenderManager::Instance()->SetUniformMatrix4fv(modelLocation, modelMatrix, false);

	model->Render(shadowShader);
}

ShadowSystem::~ShadowSystem()
{
}

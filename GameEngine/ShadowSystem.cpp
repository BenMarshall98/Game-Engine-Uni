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

	glCullFace(GL_FRONT);

	Directional * const directional = LightManager::Instance()->GetDirectionalLight();

	directional->perspective = glm::ortho(mTopLeftCoord.x, mBottomRightCoord.x,
		mBottomRightCoord.y, mTopLeftCoord.y, mTopLeftCoord.z, mBottomRightCoord.z);
	
	directional->view = glm::lookAt(camera->GetPosition() + directional->direction, camera->GetPosition(), glm::vec3(0, 0, 1));

	glViewport(0, 0, LightManager::shadowWidth, LightManager::shadowHeight);
	const ShadowFrameBuffer FrameBuffer = LightManager::Instance()->GetDirectionalFramebuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer.FrameBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < models.size(); i++)
	{
		RenderDirectional(models.at(i), positions.at(i), directions.at(i), directional, directionalShadowShaders.at(i));
	}

	std::vector<PointLight *> pointLights = LightManager::Instance()->GetRenderPointLights();

	for (int i = 0; i < pointLights.size(); i++)
	{
		ShadowFrameBuffer * const FrameBuffer = pointLights[i]->shadowTexture;

		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer->FrameBuffer);
		glClear(GL_DEPTH_BUFFER_BIT);

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

		int lastShader = -1;

		for (int j = 0; j < models.size(); j++)
		{
			if (lastShader != pointShadowShaders.at(j)->ShaderID())
			{
				pointShadowShaders.at(j)->UseShader();

				int lightPosLocation = glGetUniformLocation(pointShadowShaders.at(j)->ShaderID(), "lightPos");
				glUniform3fv(lightPosLocation, 1, value_ptr(pointLights[i]->location));

				int farPlaneLocation = glGetUniformLocation(pointShadowShaders.at(j)->ShaderID(), "farPlane");
				glUniform1f(farPlaneLocation, farPlane);

				for (int k = 0; k < shadowTransforms.size(); k++)
				{
					int viewLocation = glGetUniformLocation(pointShadowShaders.at(j)->ShaderID(), views.at(k).c_str());
					glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(shadowTransforms.at(k)));
				}

				lastShader = pointShadowShaders.at(j)->ShaderID();
			}

			RenderPoint(models.at(j), positions.at(j), directions.at(j), pointShadowShaders.at(j));
		}
	}

	std::vector<SpotLight *> spotLights = LightManager::Instance()->GetRenderSpotLights();

	for (int i = 0; i < spotLights.size(); i++)
	{
		ShadowFrameBuffer * const FrameBuffer = spotLights[i]->shadowTexture;

		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer->FrameBuffer);
		glClear(GL_DEPTH_BUFFER_BIT);

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

		int lastShader = -1;

		for (int j = 0; j < models.size(); j++)
		{
			if (lastShader != pointShadowShaders.at(j)->ShaderID())
			{
				pointShadowShaders.at(j)->UseShader();

				int lightPosLocation = glGetUniformLocation(pointShadowShaders.at(j)->ShaderID(), "lightPos");
				glUniform3fv(lightPosLocation, 1, value_ptr(spotLights[i]->location));

				int farPlaneLocation = glGetUniformLocation(pointShadowShaders.at(j)->ShaderID(), "farPlane");
				glUniform1f(farPlaneLocation, farPlane);

				for (int k = 0; k < shadowTransforms.size(); k++)
				{
					const std::string view = views.at(k).c_str();
					int viewLocation = glGetUniformLocation(pointShadowShaders.at(j)->ShaderID(), view.c_str());
					glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(shadowTransforms.at(k)));
				}

				lastShader = pointShadowShaders.at(j)->ShaderID();
			}

			RenderPoint(models.at(j), positions.at(j), directions.at(j), pointShadowShaders.at(j));
		}
	}

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

void ShadowSystem::RenderDirectional(iModel * const model, const glm::vec3 & position, const glm::quat & direction, Directional * const directional, Shader * const shadowShader)
{
	shadowShader->UseShader();

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix *= toMat4(direction);

	int modelLocation = glGetUniformLocation(shadowShader->ShaderID(), "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelMatrix));

	int perspectiveLocation = glGetUniformLocation(shadowShader->ShaderID(), "perspective");
	glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, value_ptr(directional->perspective));

	int viewLocation = glGetUniformLocation(shadowShader->ShaderID(), "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(directional->view));

	model->Render(shadowShader);
	glUseProgram(0);
}

void ShadowSystem::RenderPoint(iModel * const model, const glm::vec3 & position, const glm::quat & direction, Shader * const shadowShader)
{
	shadowShader->UseShader();

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix *= toMat4(direction);

	int modelLocation = glGetUniformLocation(shadowShader->ShaderID(), "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelMatrix));

	model->Render(shadowShader);
}

ShadowSystem::~ShadowSystem()
{
}

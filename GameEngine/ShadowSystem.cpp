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

ShadowSystem::ShadowSystem(vec3 & topLeftCoord, vec3 & bottomRightCoord) :
	mTopLeftCoord(topLeftCoord), mBottomRightCoord(bottomRightCoord), entityManager(EntityManager::Instance())
{
	ComponentType componentTypes[] = { ComponentType::COMPONENT_MODEL, ComponentType::COMPONENT_POSITION, ComponentType::COMPONENT_DIRECTION, ComponentType::COMPONENT_SHADOW_SHADER};
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, size(componentTypes));

	for (int i = 0; i < 6; i++)
	{
		string view = "shadowView[" + to_string(i) + ']';
		views.push_back(view);
	}
}

void ShadowSystem::RemoveEntity(Entity * pEntity)
{
	vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void ShadowSystem::Action(void)
{
	vector<iModel *> models;
	vector<vec3> positions;
	vector<quat> directions;
	vector<Shader *> directionalShadowShaders;
	vector<Shader *> pointShadowShaders;

	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentModel = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_MODEL);
		iComponent * componentPosition = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_POSITION);
		iComponent * componentDirection = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_DIRECTION);
		iComponent * componentShadowShader = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_SHADOW_SHADER);

		iModel * model = dynamic_cast<ComponentModel *>(componentModel)->GetRenderModel();
		vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetRenderPosition();
		quat direction = dynamic_cast<ComponentDirection *>(componentDirection)->GetRenderDirection();
		Shader * directionalShadowShader = dynamic_cast<ComponentShadowShader *>(componentShadowShader)->GetDirectionalShader();
		Shader * pointShadowShader = dynamic_cast<ComponentShadowShader *>(componentShadowShader)->GetPointShader();

		models.push_back(model);
		positions.push_back(position);
		directions.push_back(direction);
		directionalShadowShaders.push_back(directionalShadowShader);
		pointShadowShaders.push_back(pointShadowShader);
	}

	glCullFace(GL_FRONT);

	Camera * camera = CameraManager::Instance()->GetCamera();
	Directional * directional = LightManager::Instance()->GetDirectionalLight();

	directional->perspective = ortho(mTopLeftCoord.x, mBottomRightCoord.x,
		mBottomRightCoord.y, mTopLeftCoord.y, mTopLeftCoord.z, mBottomRightCoord.z);
	
	directional->view = lookAt(camera->GetPosition() + directional->direction, camera->GetPosition(), vec3(0, 0, 1));

	glViewport(0, 0, LightManager::shadowWidth, LightManager::shadowHeight);
	ShadowFrameBuffer FrameBuffer = LightManager::Instance()->GetDirectionalFramebuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer.FrameBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < EntityList.size(); i++)
	{
		RenderDirectional(models.at(i), positions.at(i), directions.at(i), directional, directionalShadowShaders.at(i));
	}

	vector<PointLight *> pointLights = LightManager::Instance()->GetRenderPointLights();

	for (int i = 0; i < pointLights.size(); i++)
	{
		ShadowFrameBuffer * FrameBuffer = pointLights[i]->shadowTexture;

		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer->FrameBuffer);
		glClear(GL_DEPTH_BUFFER_BIT);

		float aspect = (float)LightManager::shadowWidth / (float)LightManager::shadowHeight;
		float farPlane = 25.0f;

		pointLights[i]->farPlane = farPlane;
		mat4 projection = perspective(radians(90.0f), aspect, 1.0f, farPlane);

		vector<mat4> shadowTransforms;
		shadowTransforms.push_back(projection * lookAt(pointLights[i]->location, pointLights[i]->location + vec3(1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(projection * lookAt(pointLights[i]->location, pointLights[i]->location + vec3(-1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(projection * lookAt(pointLights[i]->location, pointLights[i]->location + vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(projection * lookAt(pointLights[i]->location, pointLights[i]->location + vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(projection * lookAt(pointLights[i]->location, pointLights[i]->location + vec3(0.0, 0.0, 1.0), vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(projection * lookAt(pointLights[i]->location, pointLights[i]->location + vec3(0.0, 0.0, -1.0), vec3(0.0, -1.0, 0.0)));

		int lastShader = -1;

		for (int j = 0; j < EntityList.size(); j++)
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

	vector<SpotLight *> spotLights = LightManager::Instance()->GetRenderSpotLights();

	for (int i = 0; i < spotLights.size(); i++)
	{
		ShadowFrameBuffer * FrameBuffer = spotLights[i]->shadowTexture;

		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer->FrameBuffer);
		glClear(GL_DEPTH_BUFFER_BIT);

		float aspect = (float)LightManager::shadowWidth / (float)LightManager::shadowHeight;
		float farPlane = 25.0f;

		spotLights[i]->farPlane = farPlane;
		mat4 projection = perspective(radians(90.0f), aspect, 1.0f, farPlane);

		vector<mat4> shadowTransforms;
		shadowTransforms.push_back(projection * lookAt(spotLights[i]->location, spotLights[i]->location + vec3(1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(projection * lookAt(spotLights[i]->location, spotLights[i]->location + vec3(-1.0, 0.0, 0.0), vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(projection * lookAt(spotLights[i]->location, spotLights[i]->location + vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(projection * lookAt(spotLights[i]->location, spotLights[i]->location + vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(projection * lookAt(spotLights[i]->location, spotLights[i]->location + vec3(0.0, 0.0, 1.0), vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(projection * lookAt(spotLights[i]->location, spotLights[i]->location + vec3(0.0, 0.0, -1.0), vec3(0.0, -1.0, 0.0)));

		int lastShader = -1;

		for (int j = 0; j < EntityList.size(); j++)
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
					string view = views.at(k).c_str();
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

void ShadowSystem::RenderDirectional(iModel * model, vec3 & position, quat & direction, Directional * directional, Shader * shadowShader)
{
	shadowShader->UseShader();

	mat4 modelMatrix(1.0f);
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

void ShadowSystem::RenderPoint(iModel * model, vec3 & position, quat & direction, Shader * shadowShader)
{
	shadowShader->UseShader();

	mat4 modelMatrix(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix *= toMat4(direction);

	int modelLocation = glGetUniformLocation(shadowShader->ShaderID(), "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelMatrix));

	model->Render(shadowShader);
}

ShadowSystem::~ShadowSystem()
{
}

#define GLM_ENABLE_EXPERIMENTAL

#include "ShadowSystem.h"
#include "ComponentDirection.h"
#include "ComponentPosition.h"
#include "ComponentModel.h"
#include "CameraManager.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include <algorithm>

ShadowSystem::ShadowSystem(vec3 & topLeftCoord, vec3 & bottomRightCoord, Shader * directionalShadow, Shader * pointShadow) : 
	mTopLeftCoord(topLeftCoord), mBottomRightCoord(bottomRightCoord), mDirectionalShadow(directionalShadow), mPointShadow(pointShadow)
{
	entityManager = EntityManager::Instance();
	ComponentType componentTypes[] = { ComponentType::COMPONENT_MODEL, ComponentType::COMPONENT_POSITION, ComponentType::COMPONENT_DIRECTION };
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, size(componentTypes));
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
		iComponent * componentModel = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_MODEL);
		iComponent * componentPosition = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_POSITION);
		iComponent * componentDirection = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_DIRECTION);

		const iModel * model = dynamic_cast<ComponentModel *>(componentModel)->GetRenderModel();
		vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetRenderPosition();
		quat direction = dynamic_cast<ComponentDirection *>(componentDirection)->GetRenderDirection();

		RenderDirectional(model, position, direction, directional);
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

		for (int j = 0; j < EntityList.size(); j++)
		{
			iComponent * componentModel = entityManager->GetComponentOfEntity(EntityList[j], ComponentType::COMPONENT_MODEL);
			iComponent * componentPosition = entityManager->GetComponentOfEntity(EntityList[j], ComponentType::COMPONENT_POSITION);
			iComponent * componentDirection = entityManager->GetComponentOfEntity(EntityList[j], ComponentType::COMPONENT_DIRECTION);

			const iModel * model = dynamic_cast<ComponentModel *>(componentModel)->GetRenderModel();
			vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetRenderPosition();
			quat direction = dynamic_cast<ComponentDirection *>(componentDirection)->GetRenderDirection();

			RenderPoint(model, position, direction, shadowTransforms, pointLights[i]->location, pointLights[i]->farPlane);
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

		for (int j = 0; j < EntityList.size(); j++)
		{
			iComponent * componentModel = entityManager->GetComponentOfEntity(EntityList[j], ComponentType::COMPONENT_MODEL);
			iComponent * componentPosition = entityManager->GetComponentOfEntity(EntityList[j], ComponentType::COMPONENT_POSITION);
			iComponent * componentDirection = entityManager->GetComponentOfEntity(EntityList[j], ComponentType::COMPONENT_DIRECTION);

			const iModel * model = dynamic_cast<ComponentModel *>(componentModel)->GetRenderModel();
			vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetRenderPosition();
			quat direction = dynamic_cast<ComponentDirection *>(componentDirection)->GetRenderDirection();

			RenderPoint(model, position, direction, shadowTransforms, spotLights[i]->location, spotLights[i]->farPlane);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

void ShadowSystem::RenderDirectional(const iModel * model, vec3 & position, quat & direction, Directional * directional)
{
	mDirectionalShadow->UseShader();

	mat4 modelMatrix(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix *= toMat4(direction);

	int modelLocation = glGetUniformLocation(mDirectionalShadow->ShaderID(), "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelMatrix));

	int perspectiveLocation = glGetUniformLocation(mDirectionalShadow->ShaderID(), "perspective");
	glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, value_ptr(directional->perspective));

	int viewLocation = glGetUniformLocation(mDirectionalShadow->ShaderID(), "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(directional->view));

	model->Render(mDirectionalShadow);
	glUseProgram(0);
}

void ShadowSystem::RenderPoint(const iModel * model, vec3 & position, quat & direction, vector<mat4> views, vec3 & lightPos, float farPlane)
{
	mPointShadow->UseShader();

	mat4 modelMatrix(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix *= toMat4(direction);

	int modelLocation = glGetUniformLocation(mPointShadow->ShaderID(), "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelMatrix));

	int lightPosLocation = glGetUniformLocation(mPointShadow->ShaderID(), "lightPos");
	glUniform3fv(lightPosLocation, 1, value_ptr(lightPos));

	int farPlaneLocation = glGetUniformLocation(mPointShadow->ShaderID(), "farPlane");
	glUniform1f(farPlaneLocation, farPlane);

	for (int i = 0; i < views.size(); i++)
	{
		string view = "shadowView[" + i + ']';
		int viewLocation = glGetUniformLocation(mPointShadow->ShaderID(), view.c_str());
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(views[i]));
	}

	model->Render(mDirectionalShadow);
	glUseProgram(0);
}

ShadowSystem::~ShadowSystem()
{
}

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

	glViewport(0, 0, LightManager::shadowHeight, LightManager::shadowWidth);
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

ShadowSystem::~ShadowSystem()
{
}

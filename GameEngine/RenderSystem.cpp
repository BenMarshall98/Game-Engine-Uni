#include "RenderSystem.h"
#include "ComponentModel.h"
#include "ComponentPosition.h"
#include "ComponentShader.h"
#include "ComponentTexture.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFWWindow.h"
#include <string>

RenderSystem::RenderSystem(EntityManager & pEntityManager, Camera * pCamera, Projection * pProjection) : entityManager(pEntityManager), camera(pCamera), projection(pProjection)
{
	string componentTypes[] = { "ComponentModel", "ComponentShader", "ComponentPosition", "ComponentTexture" };
	EntityList = entityManager.GetAllEntitiesWithComponents(componentTypes, size(componentTypes));
}

void RenderSystem::Action(void)
{
	mat4 perspectiveMatrix = projection->GetProjection();
	mat4 viewMatrix = camera->GetViewMatrix();
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentShader = entityManager.GetComponentOfEntity(EntityList[i], "ComponentShader");
		iComponent * componentModel = entityManager.GetComponentOfEntity(EntityList[i], "ComponentModel");
		iComponent * componentPosition = entityManager.GetComponentOfEntity(EntityList[i], "ComponentPosition");
		iComponent * componentTexture = entityManager.GetComponentOfEntity(EntityList[i], "ComponentTexture");

		Shader * shader = ((ComponentShader *)componentShader)->GetShader();
		iModel * model = ((ComponentModel *)componentModel)->GetModel();
		vec3 position = ((ComponentPosition *)componentPosition)->GetPosition();
		Texture * texture = ((ComponentTexture *)componentTexture)->GetTexture();

		Render(shader, model, position, texture, perspectiveMatrix, viewMatrix);
	}
}

void RenderSystem::Render(Shader * shader, iModel * model, vec3 position, Texture * texture, mat4 perspectiveMatrix, mat4 viewMatrix)
{
	shader->UseShader();

	mat4 modelMatrix(1.0f);
	modelMatrix = translate(modelMatrix, position);
	int modelLocation = glGetUniformLocation(shader->ShaderID(), "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelMatrix));

	int perspectiveLocation = glGetUniformLocation(shader->ShaderID(), "perspective");
	glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, value_ptr(perspectiveMatrix));

	int viewLocation = glGetUniformLocation(shader->ShaderID(), "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(viewMatrix));

	int textureLocation = glGetUniformLocation(shader->ShaderID(), "texture");
	glUniform1i(textureLocation, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->TextureID());

	model->Render(shader);
	glUseProgram(0);
}

RenderSystem::~RenderSystem()
{
}

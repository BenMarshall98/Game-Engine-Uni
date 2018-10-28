#include "RenderSystem.h"
#include "ComponentModel.h"
#include "ComponentPosition.h"
#include "ComponentShader.h"
#include "glad.h"
#include <string>

RenderSystem::RenderSystem(EntityManager & pEntityManager) : entityManager(pEntityManager)
{
	string componentTypes[] = { "ComponentModel", "ComponentShader", "ComponentPosition" };
	EntityList = entityManager.GetAllEntitiesWithComponents(componentTypes, 3);
}

void RenderSystem::Action(void)
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentShader = entityManager.GetComponentOfEntity(EntityList[i], "ComponentShader");
		iComponent * componentModel = entityManager.GetComponentOfEntity(EntityList[i], "ComponentModel");
		iComponent * componentPosition = entityManager.GetComponentOfEntity(EntityList[i], "ComponentPosition");

		Shader * shader = ((ComponentShader *)componentShader)->GetShader();
		iModel * model = ((ComponentModel *)componentModel)->GetModel();
		vec3 * position = ((ComponentPosition *)componentPosition)->GetPosition();

		Render(shader, model, position);
	}
}

void RenderSystem::Render(Shader * shader, iModel * model, vec3 * position)
{
	shader->UseShader();
	int posOffLocation = glGetUniformLocation(shader->ShaderID(), "PosOff");
	glUniform3fv(posOffLocation, sizeof(vec3), (float*)position);
	model->Render();
	glUseProgram(0);
}

RenderSystem::~RenderSystem()
{
}

#include "RenderSystem.h"
#include <string>

RenderSystem::RenderSystem(EntityManager & entityManager)
{
	string componentTypes[] = { "ComponentModel", "ComponentShader", "ComponentPosition" };
	EntityList = entityManager.GetAllEntitiesWithComponents(componentTypes, componentTypes->length());
}

void RenderSystem::OnAction(void)
{
	for (int i = 0; i < EntityList.size(); i++)
	{

	}
}

void 

RenderSystem::~RenderSystem()
{
}

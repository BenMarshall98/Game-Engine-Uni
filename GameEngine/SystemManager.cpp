#include "SystemManager.h"



SystemManager::SystemManager()
{
}


SystemManager::~SystemManager()
{
}

void SystemManager::AddRenderSystem(iSystem * system)
{
	RenderSystems.push_back(system);
}

void SystemManager::AddUpdateSystem(iSystem * system)
{
	UpdateSystems.push_back(system);
}

void SystemManager::Render()
{
	for (int i = 0; i < RenderSystems.size(); i++)
	{
		RenderSystems.at(i)->Action();
	}
}

void SystemManager::Update()
{
	for (int i = 0; i < UpdateSystems.size(); i++)
	{
		UpdateSystems.at(i)->Action();
	}
}

void SystemManager::ClearSystems()
{
	for (int i = 0; i < RenderSystems.size(); i++)
	{
		delete RenderSystems.at(i);
	}

	RenderSystems.clear();

	for (int i = 0; i < UpdateSystems.size(); i++)
	{
		delete UpdateSystems.at(i);
	}

	UpdateSystems.clear();
}

void SystemManager::RemoveEntityFromSystems(Entity * pEntity)
{
	for (int i = 0; i < RenderSystems.size(); i++)
	{
		RenderSystems.at(i)->RemoveEntity(pEntity);
	}
	for (int i = 0; i < UpdateSystems.size(); i++)
	{
		UpdateSystems.at(i)->RemoveEntity(pEntity);
	}
}
#include "SystemManager.h"

SystemManager * SystemManager::instance = nullptr;

SystemManager::SystemManager()
{
}


SystemManager::~SystemManager()
{
	Clear();
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

void SystemManager::Clear()
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

void SystemManager::RemoveEntityFromSystems(Entity * const pEntity)
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

void SystemManager::AddEntityToSystems(Entity * const pEntity)
{
	for (int i = 0; i < RenderSystems.size(); i++)
	{
		RenderSystems.at(i)->AddEntity(pEntity);
	}

	for (int i = 0; i < UpdateSystems.size(); i++)
	{
		UpdateSystems.at(i)->AddEntity(pEntity);
	}
}
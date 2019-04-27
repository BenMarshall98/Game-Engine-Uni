#include "SystemManager.h"
#include "Window.h"

SystemManager * SystemManager::instance = nullptr;

SystemManager::SystemManager()
{
}


SystemManager::~SystemManager()
{
	Clear();
}

//Goes through render systems
void SystemManager::Render()
{
	for (int i = 0; i < RenderSystems.size(); i++)
	{
		RenderSystems.at(i)->Action(1 / (float)Window::GetFrameRate());
	}
}

//Goes through update systems
void SystemManager::Update()
{
	for (int i = 0; i < UpdateSystems.size(); i++)
	{
		UpdateSystems.at(i)->Action(1 / (float)Window::GetFrameRate());
	}
}

//Removes all systems 
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

//Removes entity from all systems
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

//Adds/update entity to all systems
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
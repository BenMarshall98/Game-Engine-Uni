#pragma once

#include <vector>
#include "iSystem.h"
#include "Entity.h"

class SystemManager
{
private:
	std::vector<iSystem *> RenderSystems;
	std::vector<iSystem *> UpdateSystems;

	static SystemManager * instance;

	SystemManager();

public:
	static inline SystemManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new SystemManager();
		}

		return instance;
	}

	~SystemManager();

	inline void AddRenderSystem(iSystem * const system)
	{
		RenderSystems.push_back(system);
	}

	inline void AddUpdateSystem(iSystem * const system)
	{
		UpdateSystems.push_back(system);
	}


	void Render();
	void Update();

	void Clear();

	void RemoveEntityFromSystems(Entity * pEntity);
	void AddEntityToSystems(Entity * pEntity);
};


#pragma once

#include <vector>
#include "iSystem.h"
#include "Entity.h"

using namespace std;

class SystemManager
{
private:
	vector<iSystem *> RenderSystems;
	vector<iSystem *> UpdateSystems;

	static SystemManager * instance;

	SystemManager();

public:
	static inline  SystemManager * Instance()
	{
		if (instance == nullptr)
		{
			instance == new SystemManager();
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

	void ClearSystems();

	void RemoveEntityFromSystems(Entity * pEntity);
};


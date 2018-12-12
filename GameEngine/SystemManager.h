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

public:
	SystemManager();
	~SystemManager();

	void AddRenderSystem(iSystem * system);
	void AddUpdateSystem(iSystem * system);

	void Render();
	void Update();

	void ClearSystems();

	void RemoveEntityFromSystems(Entity * pEntity);
};


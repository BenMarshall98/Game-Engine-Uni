#pragma once

#include "iComponent.h"
#include "ResourceManager.h"

class ComponentModel : public iComponent
{
private:
	iModel * model;

public:
	explicit ComponentModel(string & modelName) : model(ResourceManager::GetModel(modelName))
	{
	}

	ComponentModel& operator=(const ComponentModel&) = delete;
	ComponentModel(ComponentModel&) = delete;

	~ComponentModel();

	ComponentType GetComponentName() override;

	void RenderSwap() override;

	inline iModel * GetRenderModel()
	{
		return model;
	}
};


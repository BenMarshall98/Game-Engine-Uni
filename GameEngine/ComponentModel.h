#pragma once

#include "iComponent.h"
#include "ResourceManager.h"

class ComponentModel : public iComponent
{
private:
	iModel * model;

public:
	explicit ComponentModel(string modelName)
	{
		model = ResourceManager::GetModel(modelName);
	}

	~ComponentModel()
	{
		delete model;
	}

	ComponentType GetComponentName() override;

	inline iModel * GetModel()
	{
		return model;
	}
};


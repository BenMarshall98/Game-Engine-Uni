#pragma once

#include "iComponent.h"
#include "ResourceManager.h"

class ComponentModel : public iComponent
{
private:
	iModel * model;

public:
	ComponentModel(string modelName)
	{
		model = ResourceManager::GetModel(modelName);
	}

	~ComponentModel()
	{
		delete model;
	}

	string GetComponentName();
	void Swap() {}

	inline iModel * GetModel()
	{
		return model;
	}
};


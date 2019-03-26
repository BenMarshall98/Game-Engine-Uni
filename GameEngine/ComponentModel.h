#pragma once

#include "iComponent.h"
#include "ResourceManager.h"

class ComponentModel : public iComponent
{
private:
	iModel * model;

public:
	explicit ComponentModel(const std::string & modelName) :
		iComponent(ComponentType::COMPONENT_MODEL), model(ResourceManager::GetModel(modelName))
	{
	}

	ComponentModel& operator=(const ComponentModel&) = delete;
	ComponentModel(ComponentModel&) = delete;

	~ComponentModel();

	void RenderSwap() override;

	inline iModel * GetRenderModel() const
	{
		return model;
	}
};


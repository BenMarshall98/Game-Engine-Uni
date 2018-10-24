#pragma once

#include "iComponent.h"
#include "iModel.h"

class ComponentModel : public iComponent
{
private:
	iModel * model;
public:
	ComponentModel(iModel * pModel) : model(pModel) {}

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


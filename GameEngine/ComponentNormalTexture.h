#pragma once

#include "iComponent.h"
#include "ResourceManager.h"

class ComponentNormalTexture : public iComponent
{
private:
	Texture * texture;

public:
	explicit ComponentNormalTexture(string textureName)
	{
		texture = ResourceManager::GetTexture(textureName);
	}

	~ComponentNormalTexture()
	{
		delete texture;
	}

	ComponentType GetComponentName() override;
	void Swap() override {}

	inline Texture * GetTexture()
	{
		return texture;
	}
};
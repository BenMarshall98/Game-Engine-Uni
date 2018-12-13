#pragma once

#include "iComponent.h"
#include "ResourceManager.h"

class ComponentNormalTexture : public iComponent
{
private:
	Texture * texture;

public:
	ComponentNormalTexture(string textureName)
	{
		texture = ResourceManager::GetTexture(textureName);
	}

	~ComponentNormalTexture()
	{
		delete texture;
	}

	ComponentType GetComponentName();
	void Swap() {}

	inline Texture * GetTexture()
	{
		return texture;
	}
};
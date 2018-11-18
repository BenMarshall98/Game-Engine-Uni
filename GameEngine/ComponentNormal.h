#pragma once

#include "iComponent.h"
#include "ResourceManager.h"

class ComponentNormal : public iComponent
{
private:
	Texture * texture;

public:
	ComponentNormal(string textureName)
	{
		texture = ResourceManager::GetTexture(textureName);
	}

	~ComponentNormal()
	{
		delete texture;
	}

	string GetComponentName();
	void Swap() {}

	inline Texture * GetTexture()
	{
		return texture;
	}
};
#pragma once
#include "iComponent.h"
#include "ResourceManager.h"

class ComponentTexture : public iComponent
{
private:
	Texture * texture;

public:
	ComponentTexture(string textureName)
	{
		texture = ResourceManager::GetTexture(textureName);
	}

	~ComponentTexture()
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


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

	ComponentNormalTexture& operator= (const ComponentNormalTexture&) = delete;
	ComponentNormalTexture(ComponentNormalTexture&);

	~ComponentNormalTexture()
	{
		delete texture;
	}

	ComponentType GetComponentName() override;

	void RenderSwap() override {};

	inline Texture * GetRenderTexture()
	{
		return texture;
	}
};
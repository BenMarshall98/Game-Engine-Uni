#pragma once
#include "iComponent.h"
#include "ResourceManager.h"

class ComponentTexture : public iComponent
{
private:
	Texture * texture;

public:
	explicit ComponentTexture(string & textureName) : texture(ResourceManager::GetTexture(textureName))
	{
	}

	ComponentTexture& operator=(const ComponentTexture&) = delete;
	ComponentTexture(ComponentTexture&) = delete;

	~ComponentTexture();

	ComponentType GetComponentName() override;
	void RenderSwap() override;

	inline Texture * GetRenderTexture() const
	{
		return texture;
	}
};


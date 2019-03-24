#pragma once

#include "iComponent.h"
#include "ResourceManager.h"

class ComponentNormalTexture : public iComponent
{
private:
	Texture * texture;

public:
	explicit ComponentNormalTexture(const std::string & textureName) : texture(ResourceManager::GetTexture(textureName))
	{
	}

	ComponentNormalTexture& operator= (const ComponentNormalTexture&) = delete;
	ComponentNormalTexture(ComponentNormalTexture&) = delete;

	~ComponentNormalTexture();

	ComponentType GetComponentName() override;

	void RenderSwap() override;

	inline Texture * GetRenderTexture() const
	{
		return texture;
	}
};
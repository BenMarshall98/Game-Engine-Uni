#pragma once

#include "iComponent.h"
#include "ResourceManager.h"

class ComponentNormalTexture : public iComponent
{
private:
	Texture * texture;

public:
	explicit ComponentNormalTexture(const std::string & textureName) :
		iComponent(ComponentType::COMPONENT_NORMAL_TEXTURE), texture(ResourceManager::GetTexture(textureName))
	{
	}

	ComponentNormalTexture& operator= (const ComponentNormalTexture&) = delete;
	ComponentNormalTexture(ComponentNormalTexture&) = delete;

	~ComponentNormalTexture();

	void RenderSwap() override;

	//Gets the render thread texture
	inline Texture * GetRenderTexture() const
	{
		return texture;
	}
};
#pragma once
#include "iComponent.h"
#include "ResourceManager.h"

class ComponentTexture : public iComponent
{
private:
	Texture * texture;

public:
	explicit ComponentTexture(const std::string & textureName) :
		iComponent(ComponentType::COMPONENT_TEXTURE), texture(ResourceManager::GetTexture(textureName))
	{
	}

	ComponentTexture& operator=(const ComponentTexture&) = delete;
	ComponentTexture(ComponentTexture&) = delete;

	~ComponentTexture();

	void RenderSwap() override;

	inline Texture * GetRenderTexture() const
	{
		return texture;
	}
};


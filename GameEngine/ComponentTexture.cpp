#include "ComponentTexture.h"

ComponentTexture::~ComponentTexture()
{
	ResourceManager::RemoveTexture(texture);
}

ComponentType ComponentTexture::GetComponentName()
{
	return ComponentType::COMPONENT_TEXTURE;
}

void ComponentTexture::RenderSwap()
{

}

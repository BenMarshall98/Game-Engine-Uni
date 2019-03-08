#include "ComponentNormalTexture.h"

ComponentNormalTexture::~ComponentNormalTexture()
{
	ResourceManager::RemoveTexture(texture);
}

ComponentType ComponentNormalTexture::GetComponentName()
{
	return ComponentType::COMPONENT_NORMAL_TEXTURE;
}


void ComponentNormalTexture::RenderSwap()
{

}
#include "ComponentNormalTexture.h"

ComponentNormalTexture::~ComponentNormalTexture()
{
	delete texture;
}

ComponentType ComponentNormalTexture::GetComponentName()
{
	return ComponentType::COMPONENT_NORMAL_TEXTURE;
}


void ComponentNormalTexture::RenderSwap()
{

}
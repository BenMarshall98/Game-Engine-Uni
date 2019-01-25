#include "ComponentNormalTexture.h"

ComponentNormalTexture::~ComponentNormalTexture()
{
	delete texture;
}

ComponentType ComponentNormalTexture::GetComponentName()
{
	return ComponentType::COMPONENT_NORMAL;
}


void ComponentNormalTexture::RenderSwap()
{

}
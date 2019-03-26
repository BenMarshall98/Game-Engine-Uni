#include "ComponentNormalTexture.h"

ComponentNormalTexture::~ComponentNormalTexture()
{
	ResourceManager::RemoveTexture(texture);
}

void ComponentNormalTexture::RenderSwap()
{

}
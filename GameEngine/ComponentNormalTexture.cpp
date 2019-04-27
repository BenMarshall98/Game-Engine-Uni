#include "ComponentNormalTexture.h"

ComponentNormalTexture::~ComponentNormalTexture()
{
	ResourceManager::RemoveTexture(texture);
}

//Does nothing
void ComponentNormalTexture::RenderSwap()
{

}
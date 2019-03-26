#include "ComponentTexture.h"

ComponentTexture::~ComponentTexture()
{
	ResourceManager::RemoveTexture(texture);
}

void ComponentTexture::RenderSwap()
{

}

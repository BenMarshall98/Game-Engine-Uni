#include "ComponentTexture.h"

ComponentTexture::~ComponentTexture()
{
	ResourceManager::RemoveTexture(texture);
}

//Does nothing
void ComponentTexture::RenderSwap()
{

}

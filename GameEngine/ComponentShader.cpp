#include "ComponentShader.h"

ComponentShader::~ComponentShader()
{
	ResourceManager::RemoveShader(shader);
}

//Does nothing
void ComponentShader::RenderSwap()
{

}
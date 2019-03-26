#include "ComponentShader.h"

ComponentShader::~ComponentShader()
{
	ResourceManager::RemoveShader(shader);
}

void ComponentShader::RenderSwap()
{

}
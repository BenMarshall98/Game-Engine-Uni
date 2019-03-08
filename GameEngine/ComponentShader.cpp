#include "ComponentShader.h"

ComponentShader::~ComponentShader()
{
	ResourceManager::RemoveShader(shader);
}

ComponentType ComponentShader::GetComponentName()
{
	return ComponentType::COMPONENT_SHADER;
}

void ComponentShader::RenderSwap()
{

}
#include "RenderManager.h"

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

RenderManager * RenderManager::instance = nullptr;

RenderEngine * RenderManager::engine = nullptr;

std::string RenderManager::engineName = "OpenGL330";

#include "RenderManager.h"
#include "OpenGL330Engine.h"


RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

RenderManager * RenderManager::instance = nullptr;

RenderEngine * RenderManager::engine = new OpenGL330Engine();

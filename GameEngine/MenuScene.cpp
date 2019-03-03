#include "MenuScene.h"
#include "MenuLoader.h"
#include "OpenGL.h"
#include "Window.h"

#undef LoadMenu

MenuScene::MenuScene()
{
}


MenuScene::~MenuScene()
{
}

void MenuScene::Load()
{
	menuItems = MenuLoader::LoadMenu("GameChoosing.json", backgroundColour);
}

void MenuScene::Update()
{

}

void MenuScene::Render()
{
	glClearColor(backgroundColour.x, backgroundColour.y, backgroundColour.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < menuItems.size(); i++)
	{
		menuItems.at(i)->Render();
	}
}

void MenuScene::Resize(int width, int height)
{

}

void MenuScene::Close()
{

}
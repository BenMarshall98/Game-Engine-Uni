#include "MenuScene.h"
#include "MenuLoader.h"
#include "OpenGL.h"
#include "Window.h"
#include "SceneManager.h"
#include "RenderManager.h"
#include "ConfigLoader.h"

#undef LoadMenu

MenuScene::MenuScene(const std::string & pFileName) : iScene(pFileName)
{
}


MenuScene::~MenuScene()
{
	for (int i = 0; i < menuItems.size(); i++)
	{
		delete menuItems.at(i);
		menuItems.at(i) = nullptr;
	}

	menuItems.clear();
}

void MenuScene::Load()
{
	menuItems = MenuLoader::LoadMenu(fileName, backgroundColour);

	for (int i = 0; i < menuItems.size(); i++)
	{
		if (menuItems.at(i)->GetMenuType() == MenuType::Button)
		{
			totalSelectable++;
		}
	}
}

void MenuScene::Update()
{
	static int delay = 10 / (60 / (float)Window::GetFrameRate());

	if (delay == 0)
	{
		delay = 10 / (60 / (float)Window::GetFrameRate());
		float inputValue = InputManager::Instance()->GetInputValue(downMenu);

		if (inputValue > 0.1)
		{
			currentSelected++;

			if (currentSelected == totalSelectable)
			{
				currentSelected--;
			}
		}
		else
		{
			inputValue = InputManager::Instance()->GetInputValue(upMenu);

			if (inputValue > 0.1)
			{
				currentSelected--;

				if (currentSelected < 0)
				{
					currentSelected++;
				}
			}
			else
			{
				inputValue = InputManager::Instance()->GetInputValue(selectMenu);

				if (inputValue > 0.1)
				{
					int selectable = currentSelected;

					for (int i = 0; i < menuItems.size(); i++)
					{
						if (menuItems.at(i)->GetMenuType() == MenuType::Button)
						{
							if (selectable == 0)
							{
								menuItems.at(i)->Select();
								break;
							}
							selectable--;
						}
					}
				}
				else
				{
					inputValue = InputManager::Instance()->GetInputValue(escapeMenu);

					if (inputValue > 0.1)
					{
						SceneManager::Instance()->CloseScene();
					}
					else
					{
						delay = 0;
					}
				}
			}
		}
	}
	else
	{
		delay--;
	}


	for (int i = 0; i < menuItems.size(); i++)
	{
		menuItems.at(i)->Update();
		menuItems.at(i)->UnHighlight();
	}

	int selectable = currentSelected;

	for (int i = 0; i < menuItems.size(); i++)
	{
		if (menuItems.at(i)->GetMenuType() == MenuType::Button)
		{
			if (selectable == 0)
			{
				menuItems.at(i)->Highlight();
				break;
			}
			selectable--;
		}
	}
}

void MenuScene::Render()
{
	RenderManager::Instance()->ClearColor(backgroundColour);
	RenderManager::Instance()->ClearDepthBuffer();
	RenderManager::Instance()->ClearColorBuffer();

	for (int i = 0; i < menuItems.size(); i++)
	{
		menuItems.at(i)->Render();
	}
}

void MenuScene::Swap()
{
	for (int i = 0; i < menuItems.size(); i++)
	{
		menuItems.at(i)->Swap();
	}
}

void MenuScene::Close()
{
	ConfigLoader::UpdateLevelConfig();
}
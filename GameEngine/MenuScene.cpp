#include "MenuScene.h"
#include "MenuLoader.h"
#include "OpenGL.h"
#include "Window.h"

#undef LoadMenu

MenuScene::MenuScene(std::string pFileName) : iScene(pFileName)
{
}


MenuScene::~MenuScene()
{
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
	static int delay = 10;

	if (delay == 0)
	{
		delay = 10;
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
					delay = 0;
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
	glClearColor(backgroundColour.x, backgroundColour.y, backgroundColour.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

}
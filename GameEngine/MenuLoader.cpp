#include "MenuLoader.h"
#include "Text.h"
#include "Button.h"
#include "ScriptingManager.h"
#include "ConfigLoader.h"
#include <fstream>

#undef LoadMenu

std::vector<MenuItem *> MenuLoader::LoadMenu(std::string & fileName, glm::vec3 & backgroundColour)
{
	std::vector<MenuItem *> menuItems;

	std::ifstream in(fileName);

	std::string fullFile;

	while (!in.eof())
	{
		const char c = in.get();
		if (c == -1)
		{
			break;
		}
		fullFile += c;
	}

	rapidjson::Document d;
	d.Parse(fullFile.c_str());

	if (d.HasParseError())
	{
		return menuItems;
	}

	if (d.HasMember("Script"))
	{
		const std::string script = d["Script"].GetString();

		ScriptingManager::Instance()->LoadLuaFromFile(script);
	}

	if (d.HasMember("Config"))
	{
		std::string config = d["Config"].GetString();

		ConfigLoader::LoadLevelConfig(config);
	}

	if (d.HasMember("Background-Colour"))
	{
		if (d["Background-Colour"].IsArray())
		{
			LoadColour(d["Background-Colour"], backgroundColour);
		}
	}

	if (d.HasMember("MenuItems"))
	{
		if (d["MenuItems"].IsArray())
		{
			LoadMenuItems(d["MenuItems"], menuItems);
		}
	}

	return menuItems;
}

void MenuLoader::LoadMenuItems(const rapidjson::Value& MenuItems, std::vector<MenuItem *> & menuItemsVec)
{
	rapidjson::Value::ConstValueIterator it;

	for (it = MenuItems.Begin(); it != MenuItems.End(); it++)
	{
		const std::string type = (*it)["Type"].GetString();
		std::string text = (*it)["Text"].GetString();
		
		const rapidjson::Value& loc = (*it)["Location"];
		glm::vec2 location = glm::vec2(0);

		for (rapidjson::SizeType i = 0; i < loc.Size(); i++)
		{
			location[i] = loc[i].GetFloat();
		}

		std::string align = (*it)["Align"].GetString();
		float fontSize = (*it)["FontSize"].GetFloat();

		glm::vec3 fontColour = glm::vec3(0);
		LoadColour((*it)["FontColour"], fontColour);

		if (type == "Text")
		{
			menuItemsVec.push_back(new Text(text, align, fontSize, fontColour, location));
		}
		else if (type == "Button")
		{
			glm::vec3 highlighed = glm::vec3(0);
			LoadColour((*it)["Highlighted"], highlighed);

			std::string function = (*it)["Function"].GetString();

			menuItemsVec.push_back(new Button(text, align, fontSize, fontColour, highlighed, location, function));
		}
	}
}

void MenuLoader::LoadColour(const rapidjson::Value& ColourLoc, glm::vec3 & colour)
{
	for (rapidjson::SizeType i = 0; i < ColourLoc.Size(); i++)
	{
		colour[i] = (ColourLoc[i].GetFloat() / 255);
	}
}

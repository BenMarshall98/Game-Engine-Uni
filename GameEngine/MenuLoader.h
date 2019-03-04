#pragma once
#include <string>
#include "glm/glm.hpp"
#include "MenuItem.h"
#include <vector>
#include "RapidJSON/document.h"

#undef LoadMenu

class MenuLoader
{
private:
	static void LoadMenuItems(const rapidjson::Value& MenuItems, std::vector<MenuItem *> & menuItemsVec);
	static void LoadColour(const rapidjson::Value& ColourLoc, glm::vec3 & colour);

public:
	static std::vector<MenuItem *> LoadMenu(std::string fileName, glm::vec3 & backgroundColour);
	
};


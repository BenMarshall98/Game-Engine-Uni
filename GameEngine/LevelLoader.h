#pragma once

#include "Entity.h"
#include "RapidJSON/document.h"
#include "glm/glm.hpp"
#include "InputFunction.h"
#include "iSystem.h"
#include <string>
#include <map>

class LevelLoader
{
private:
	LevelLoader() {};

	static std::map<std::string, const rapidjson::Value&> templates;

	static void LoadResourcesJSON(const rapidjson::Value& Resources);
	static void LoadScriptsJSON(const rapidjson::Value& Scripts);
	static void LoadEntityTemplatesJSON(const rapidjson::Value& EntityTemplates);
	static void LoadEntity(const rapidjson::Value& Entities);
	static void LoadLights(const rapidjson::Value& Lights);
	static void LoadMapJSON(const rapidjson::Value& Map);
	static void LoadViewJSON(const rapidjson::Value& View);
	static void LoadPerspectiveJSON(const rapidjson::Value& Perspective);
	static void LoadCameraJSON(const rapidjson::Value& Camera);
	static void LoadPlatformerMap(const std::string & file, const glm::vec2 & topLeftCoord);
	static void Load3DMap(const std::string & file, const glm::vec2 & topLeftCoord);
	static void LoadSystems(const rapidjson::Value& Systems);
	static std::vector<iSystem *> CreateSystems(const rapidjson::Value& Systems);
	static void AddComponentsToEntityJSON(Entity * entity, const rapidjson::Value& components);
	static std::vector<InputFunction *> * LoadInputsJSON(const rapidjson::Value& Inputs);
	static int GetLine(std::string & file, int location);
public:
	static void LoadLevelJSON(std::string & fileName);
	
};


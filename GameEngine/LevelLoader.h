#pragma once

#include "Entity.h"
#include "RapidJSON/document.h"
#include "glm/glm.hpp"
#include "InputFunction.h"
#include "iSystem.h"
#include <string>
#include <map>

using namespace std;
using namespace glm;
using namespace rapidjson;

class LevelLoader
{
private:
	LevelLoader() {};

	static map<string, const Value&> templates;
	static void LoadResourcesJSON(const Value& Resources);
	static void LoadScriptsJSON(const Value& Scripts);
	static void LoadEntityTemplatesJSON(const Value& EntityTemplates);
	static void LoadEntity(const Value& Entities);
	static void LoadLights(const Value& Lights);
	static void LoadMapJSON(const Value& Map);
	static void LoadViewJSON(const Value& View);
	static void LoadPerspectiveJSON(const Value& Perspective);
	static void LoadCameraJSON(const Value& Camera);
	static void LoadPlatformerMap(const string & file, const vec2 & topLeftCoord);
	static void Load3DMap(const string & file, const vec2 & topLeftCoord);
	static void LoadSystems(const Value& Systems);
	static vector<iSystem *> CreateSystems(const Value& Systems);
	static void AddComponentsToEntityJSON(Entity * entity, const Value& components);
	static vector<InputFunction> * LoadInputsJSON(const Value& Inputs);
	static int GetLine(string & file, int location);
public:
	static void CoinHitPlayer(Entity * pEntity);
	static void LoadLevelJSON(string & fileName);
	
};


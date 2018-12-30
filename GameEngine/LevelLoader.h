#pragma once

#include "Entity.h"
#include "RapidJSON/document.h"
#include "glm/glm.hpp"
#include <string>
#include <map>

using namespace std;
using namespace glm;
using namespace rapidjson;

class LevelLoader
{
private:
	static map<string, const Value&> templates;
	static void LoadResourcesJSON(const Value& Resources);
	static void LoadEntityTemplatesJSON(const Value& EntityTemplates);
	static void LoadMapJSON(const Value& Map);
	static void LoadPlatformerMap(string file, string plane, vec2 topLeftCoord);
	static void AddComponentsToEntityJSON(Entity * entity, const Value& components);
public:
	static void LoadLevel(string fileName);
	static void CoinHitPlayer(Entity * pEntity);
	static void LoadLevelJSON(string fileName);
	
};


#pragma once

#include "Entity.h"
#include "RapidJSON/document.h"
#include <string>

using namespace std;
using namespace rapidjson;

class LevelLoader
{
public:
	static void LoadLevel(string fileName);
	static void CoinHitPlayer(Entity * pEntity);
	static void LoadLevelJSON(string fileName);
	static void LoadResourcesJSON(const Value& Resources);
};


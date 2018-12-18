#pragma once

#include "Entity.h"
#include <string>

using namespace std;

class LevelLoader
{
public:
	static void LoadLevel(string fileName);
	static void CoinHitPlayer(Entity * pEntity);
};


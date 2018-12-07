#pragma once

#include "EntityManager.h"
#include <string>

using namespace std;

class LevelLoader
{
public:
	static void LoadLevel(string fileName, EntityManager & entityManager);
};


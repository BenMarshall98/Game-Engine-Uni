#pragma once

#include "RapidJSON/document.h"
#include <string>

class ConfigLoader
{
private:
	static std::string engineConfigFile;
	static std::string levelConfigFile;

public:
	ConfigLoader() = delete;

	static void LoadEngineConfig(std::string & fileName);
	static void LoadLevelConfig(std::string & fileName);

	static void UpdateEngineConfig();
	static void UpdateLevelConfig();
};


#include "ConfigLoader.h"
#include "LoggingManager.h"
#include "PhysicsManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "Window.h"
#include "glm/glm.hpp"
#include <iostream>
#include <fstream>

std::string ConfigLoader::engineConfigFile = "";
std::string ConfigLoader::levelConfigFile = "";

void ConfigLoader::LoadEngineConfig(std::string & fileName)
{
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
		std::string message = "Error Loading in the Config File";
		LoggingManager::LogMessage(MESSAGE_TYPE::SEVERE, message);
		return;
	}

	std::string physicsEngine = d["PhysicsEngine"].GetString();

	const rapidjson::Value& windowSetup = d["WindowSetup"];
	std::string window = windowSetup["Window"].GetString();
	bool fullscreen = windowSetup["Fullscreen"].GetBool();

	glm::vec2 windowSize(0);

	const rapidjson::Value& size = windowSetup["Size"];

	for (rapidjson::SizeType i = 0; i < size.Size(); i++)
	{
		windowSize[i] = size[i].GetFloat();
	}

	float framerate = windowSetup["FramerateLock"].GetFloat();

	std::string graphicsDriver = d["GraphicsDriver"].GetString();
	std::string audioDriver = d["AudioDriver"].GetString();

	if (d.HasMember("LogFile"))
	{
		std::string logFile = d["LogFile"].GetString();
		SceneManager::InitFileStream(logFile);
	}

	PhysicsManager::SetPhysicsEngine(physicsEngine);
	RenderManager::SetRenderEngine(graphicsDriver);
	SceneManager::InitWindow(window);
	AudioManager::InitAudioEngineName(audioDriver);

	Window::InitFullScreen(fullscreen);
	Window::InitHeight(windowSize.y);
	Window::InitWidth(windowSize.x);
	Window::SetFrameRate(framerate);

	engineConfigFile = fileName;
}

void ConfigLoader::LoadLevelConfig(std::string & fileName)
{

}

void ConfigLoader::UpdateEngineConfig()
{

}

void ConfigLoader::UpdateLevelConfig()
{

}
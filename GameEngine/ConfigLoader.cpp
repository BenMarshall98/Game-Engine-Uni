#include "ConfigLoader.h"
#include "LoggingManager.h"
#include "PhysicsManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "Window.h"
#include "glm/glm.hpp"
#include <iostream>
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "Input.h"

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

	engineConfigFile = fileName;
}

void ConfigLoader::LoadLevelConfig(std::string & fileName)
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

	glm::vec3 gravity(0);

	const rapidjson::Value& gra = d["Gravity"];

	for (rapidjson::SizeType i = 0; i < gra.Size(); i++)
	{
		gravity[i] = gra[i].GetFloat();
	}

	std::string controller = d["Controller"].GetString();

	float framerate = d["FramerateLock"].GetFloat();

	PhysicsManager::SetGravity(gravity);
	Input::ControllerSetup(controller);
	Window::SetFrameRate(framerate);

	levelConfigFile = fileName;
}

void ConfigLoader::UpdateEngineConfig()
{
	if (engineConfigFile == "")
	{
		return;
	}

	std::ifstream in(engineConfigFile);

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

	rapidjson::Value& windowSetup = d["WindowSetup"];

	rapidjson::Value& fullscreen = windowSetup["Fullscreen"];
	fullscreen.SetBool(Window::IsFullScreen());

	glm::vec2 windowSize(Window::GetWidth(), Window::GetHeight());

	rapidjson::Value& size = windowSetup["Size"];

	for (rapidjson::SizeType i = 0; i < size.Size(); i++)
	{
		size[i].SetFloat(windowSize[i]);
	}

	std::ofstream out(engineConfigFile);

	rapidjson::OStreamWrapper outr(out);

	rapidjson::PrettyWriter <rapidjson::OStreamWrapper> writer(outr);
	d.Accept(writer);
}

void ConfigLoader::UpdateLevelConfig()
{
	if (levelConfigFile == "")
	{
		return;
	}

	std::ifstream in(levelConfigFile);

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

	rapidjson::Value& controller = d["Controller"];

	std::string s = Input::GetController().c_str();
	int len = strlen(Input::GetController().c_str());
	controller.SetString(s.c_str(), len);

	d["FramerateLock"].SetFloat(Window::GetFrameRate());

	std::ofstream out(levelConfigFile);

	rapidjson::OStreamWrapper outr(out);

	rapidjson::PrettyWriter <rapidjson::OStreamWrapper> writer(outr);
	d.Accept(writer);
}
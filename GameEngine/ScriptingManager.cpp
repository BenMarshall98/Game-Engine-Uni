#include "ScriptingManager.h"
#include "LoggingManager.h"

extern "C"
{
	#include "Lua/lua.h"
	#include "Lua/lauxlib.h"
	#include "Lua/lualib.h"
}


ScriptingManager::ScriptingManager()
{
	luaVM = luaL_newstate();

	if (luaVM = nullptr)
	{
		string message = "Failed to Initialize lua";
		LoggingManager::LogMessage(SEVERE, message);
	}

	luaL_openlibs(luaVM);
}

void ScriptingManager::LoadLuaFromFile(string file)
{
	int iStatus = luaL_loadstring(luaVM, file.c_str());
	if (iStatus)
	{
		string message = lua_tostring(luaVM, -1);
		message = "Error: " + message;
		LoggingManager::LogMessage(SEVERE, message);
	}
}



ScriptingManager::~ScriptingManager()
{
	lua_close(luaVM);
}

ScriptingManager * ScriptingManager::instance = nullptr;

#pragma once

extern "C"
{
#include "Lua/lua.h"
}

class ScriptingManager
{
	static ScriptingManager * instance;
	ScriptingManager();

	lua_State * luaVM;

public:
	static inline ScriptingManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new ScriptingManager();
		}
		return instance;
	}

	void LoadLuaFromFile(string fileName)
	{
		
	}

	~ScriptingManager();
};


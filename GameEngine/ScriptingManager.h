#pragma once

extern "C"
{
#include "Lua/lua.h"
}

#include <string>

using namespace std;

class ScriptingManager
{
	static ScriptingManager * instance;

	static int lua_GetComponentDirection(lua_State * luaState);
	static int lua_GetComponentPosition(lua_State * luaState);
	static int lua_GetComponentPhysics(lua_State * luaState);

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

	void LoadLuaFromFile(string fileName);

	~ScriptingManager();
};


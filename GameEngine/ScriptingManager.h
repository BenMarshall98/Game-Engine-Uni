#pragma once

extern "C"
{
#include "Lua/lua.h"
}

#include "Entity.h"
#include <string>

using namespace std;

class ScriptingManager
{
private:
	static ScriptingManager * instance;

	static int lua_GetComponentDirection(lua_State * luaState);
	static int lua_GetComponentPosition(lua_State * luaState);
	static int lua_GetComponentPhysics(lua_State * luaState);
	static int lua_GetPosition(lua_State * luaState);
	static int lua_SetPosition(lua_State * luaState);
	static int lua_GetVelocity(lua_State * luaState);
	static int lua_SetVelocity(lua_State * luaState);
	static int lua_GetImpulse(lua_State * luaState);
	static int lua_SetImpulse(lua_State * luaState);
	static int lua_GetTouchingGround(lua_State * luaState);

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

	void RunScriptFromFunction(string function, Entity * entity);

	void RunScriptFromInput(string function, Entity * entity, float inputValue, float deltaTime);

	~ScriptingManager();
};


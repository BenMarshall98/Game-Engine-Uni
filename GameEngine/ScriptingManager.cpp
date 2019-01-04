#include "ScriptingManager.h"
#include "LoggingManager.h"
#include "Entity.h"
#include "EntityManager.h"
#include "ComponentDirection.h"
#include "ComponentPosition.h"
#include "ComponentPhysics.h"

extern "C"
{
	#include "Lua/lua.h"
	#include "Lua/lauxlib.h"
	#include "Lua/lualib.h"
}

ScriptingManager::ScriptingManager()
{
	luaVM = luaL_newstate();

	if (luaVM == nullptr)
	{
		string message = "Failed to Initialize lua";
		LoggingManager::LogMessage(SEVERE, message);
	}

	luaL_openlibs(luaVM);

	lua_register(luaVM, "GetComponentPhysics", lua_GetComponentPhysics);
	lua_register(luaVM, "GetComponentPosition", lua_GetComponentPosition);
	lua_register(luaVM, "GetComponentDirection", lua_GetComponentDirection);
}

void ScriptingManager::LoadLuaFromFile(string file)
{
	int iStatus = luaL_loadfile(luaVM, file.c_str());
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

int ScriptingManager::lua_GetComponentDirection(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: GetComponentDirection");
		lua_error(luaState);
	}

	Entity * entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetComponentDirection");
		lua_error(luaState);
	}

	iComponent * componentDirection = EntityManager::Instance()->GetComponentOfEntity(entity, COMPONENT_DIRECTION);
	ComponentDirection * directionComponent = dynamic_cast<ComponentDirection *>(componentDirection);

	lua_pushlightuserdata(luaState, directionComponent);
	return 1;
}

int ScriptingManager::lua_GetComponentPosition(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: GetComponentPosition");
		lua_error(luaState);
	}

	Entity * entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetComponentPosition");
		lua_error(luaState);
	}

	iComponent * componentPosition = EntityManager::Instance()->GetComponentOfEntity(entity, COMPONENT_POSITION);
	ComponentPosition * positionComponent = dynamic_cast<ComponentPosition *>(componentPosition);

	lua_pushlightuserdata(luaState, positionComponent);
	return 1;
}

int ScriptingManager::lua_GetComponentPhysics(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: GetComponentPhysics");
		lua_error(luaState);
	}

	Entity * entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetComponentPhysics");
		lua_error(luaState);
	}

	iComponent * componentPhysics = EntityManager::Instance()->GetComponentOfEntity(entity, COMPONENT_PHYSICS);
	ComponentPhysics * physicsComponent = dynamic_cast<ComponentPhysics *>(componentPhysics);

	lua_pushlightuserdata(luaState, physicsComponent);
	return 1;
}

ScriptingManager * ScriptingManager::instance = nullptr;

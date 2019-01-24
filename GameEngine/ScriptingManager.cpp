#include "ScriptingManager.h"
#include "LoggingManager.h"
#include "Entity.h"
#include "EntityManager.h"
#include "ComponentDirection.h"
#include "ComponentPosition.h"
#include "ComponentPhysics.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

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
	lua_register(luaVM, "GetPosition", lua_GetPosition);
	lua_register(luaVM, "SetPosition", lua_SetPosition);
	lua_register(luaVM, "GetVelocity", lua_GetVelocity);
	lua_register(luaVM, "SetVelocity", lua_SetVelocity);
	lua_register(luaVM, "GetImpulse", lua_GetImpulse);
	lua_register(luaVM, "SetImpulse", lua_SetImpulse);
	lua_register(luaVM, "GetRotation", lua_GetRotation);
	lua_register(luaVM, "SetRotation", lua_SetRotation);
	lua_register(luaVM, "GetDirection", lua_GetDirection);
	lua_register(luaVM, "SetDirection", lua_SetDirection);
	lua_register(luaVM, "GetTouchingGround", lua_GetTouchingGround);
	lua_register(luaVM, "CreateTranslationMatrix", lua_CreateTranslationMatrix);
	lua_register(luaVM, "CreateScaleMatrix", lua_CreateScaleMatrix);
	lua_register(luaVM, "CreateRotationMatrix", lua_CreateRotationMatrix);
	lua_register(luaVM, "MultiplyMatrices", lua_MultiplyMatrices);
	lua_register(luaVM, "MultiplyMatrixVector", lua_MultiplyMatrixVector);
	lua_register(luaVM, "DeleteEntity", lua_DeleteEntity);
	
	string file = "Vector3.lua";
	LoadLuaFromFile(file);

	file = "Matrix4.lua";
	LoadLuaFromFile(file);

	file = "Quaternion.lua";
	LoadLuaFromFile(file);
}

void ScriptingManager::LoadLuaFromFile(string & file)
{
	int iStatus = luaL_loadfile(luaVM, file.c_str());
	if (iStatus)
	{
		string message = lua_tostring(luaVM, -1);
		message = "Error: " + message;
		LoggingManager::LogMessage(SEVERE, message);
	}

	if (lua_pcall(luaVM, 0, 0, 0))
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

	iComponent * componentDirection = EntityManager::Instance()->GetComponentOfEntity(entity, ComponentType::COMPONENT_DIRECTION);
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

	iComponent * componentPosition = EntityManager::Instance()->GetComponentOfEntity(entity, ComponentType::COMPONENT_POSITION);
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

	iComponent * componentPhysics = EntityManager::Instance()->GetComponentOfEntity(entity, ComponentType::COMPONENT_PHYSICS);
	ComponentPhysics * physicsComponent = dynamic_cast<ComponentPhysics *>(componentPhysics);

	lua_pushlightuserdata(luaState, physicsComponent);
	return 1;
}

int ScriptingManager::lua_GetPosition(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: GetPosition");
		lua_error(luaState);
	}
	ComponentPosition * positionComponent = (ComponentPosition *)lua_touserdata(luaState, 1);

	if (!positionComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetPosition");
		lua_error(luaState);
	}

	vec3 vector = positionComponent->GetUpdatePosition();

	lua_getglobal(luaState, "NewVector3");
	lua_pushnumber(luaState, vector.x);
	lua_pushnumber(luaState, vector.y);
	lua_pushnumber(luaState, vector.z);

	if (lua_pcall(luaState, 3, 1, 0) != 0)
	{
		string message = lua_tostring(luaState, -1);
		message = "Error running function: 'NewVector3'" + message;
		lua_pushstring(luaState, message.c_str());
		lua_error(luaState);
	}

	if (!lua_istable(luaState, -1))
	{
		lua_pushstring(luaState, "Wrong value passed back in function: 'NewVector3'");
		lua_error(luaState);
	}
	return 1;
}

int ScriptingManager::lua_SetPosition(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: SetPosition");
		lua_error(luaState);
	}

	ComponentPosition * positionComponent = (ComponentPosition *)lua_touserdata(luaState, 1);

	if (!positionComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: SetPosition");
		lua_error(luaState);
	}

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	positionComponent->SetUpdatePosition(vec3(x, y, z));

	return 0;
}

int ScriptingManager::lua_GetVelocity(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: GetVelocity");
		lua_error(luaState);
	}
	
	ComponentPhysics * physicsComponent = (ComponentPhysics *)lua_touserdata(luaState, 1);

	if (!physicsComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetVelocity");
		lua_error(luaState);
	}

	vec3 vector = physicsComponent->GetUpdateVelocity();

	lua_getglobal(luaState, "NewVector3");
	lua_pushnumber(luaState, vector.x);
	lua_pushnumber(luaState, vector.y);
	lua_pushnumber(luaState, vector.z);

	if (lua_pcall(luaState, 3, 1, 0) != 0)
	{
		string message = lua_tostring(luaState, -1);
		message = "Error running function: 'NewVector3'" + message;
		lua_pushstring(luaState, message.c_str());
		lua_error(luaState);
	}

	if (!lua_istable(luaState, -1))
	{
		lua_pushstring(luaState, "Wrong value passed back in function: 'NewVector3'");
		lua_error(luaState);
	}
	return 1;
}

int ScriptingManager::lua_SetVelocity(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: SetVelocity");
		lua_error(luaState);
	}

	ComponentPhysics * physicsComponent = (ComponentPhysics *)lua_touserdata(luaState, 1);

	if (!physicsComponent)
	{
		lua_pushstring(luaState, "Wronmg Parameters Passed in: SetVelocity");
		lua_error(luaState);
	}

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	physicsComponent->SetUpdateVelocity(vec3(x, y, z));

	return 0;
}

int ScriptingManager::lua_GetImpulse(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: GetImpluse");
		lua_error(luaState);
	}

	ComponentPhysics * physicsComponent = (ComponentPhysics *)lua_touserdata(luaState, 1);

	if (!physicsComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetImpluse");
		lua_error(luaState);
	}

	vec3 vector = physicsComponent->GetUpdateImpulse();

	lua_getglobal(luaState, "NewVector3");
	lua_pushnumber(luaState, vector.x);
	lua_pushnumber(luaState, vector.y);
	lua_pushnumber(luaState, vector.z);

	if (lua_pcall(luaState, 3, 1, 0) != 0)
	{
		string message = lua_tostring(luaState, -1);
		message = "Error running function: 'NewVector3'" + message;
		lua_pushstring(luaState, message.c_str());
		lua_error(luaState);
	}

	if (!lua_istable(luaState, -1))
	{
		lua_pushstring(luaState, "Wrong value passed back in function: 'NewVector3'");
		lua_error(luaState);
	}
	return 1;
}

int ScriptingManager::lua_SetImpulse(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: SetImpluse");
		lua_error(luaState);
	}

	ComponentPhysics * physicsComponent = (ComponentPhysics *)lua_touserdata(luaState, 1);

	if (!physicsComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: SetImpluse");
		lua_error(luaState);
	}

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	physicsComponent->SetUpdateImpulse(vec3(x, y, z));

	return 0;
}

int ScriptingManager::lua_GetRotation(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: GetRotation");
		lua_error(luaState);
	}

	ComponentPhysics * physicsComponent = (ComponentPhysics *)lua_touserdata(luaState, 1);

	if (!physicsComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetRotation");
		lua_error(luaState);
	}

	vec3 vector = physicsComponent->GetUpdateRotation();

	lua_getglobal(luaState, "NewVector3");
	lua_pushnumber(luaState, vector.x);
	lua_pushnumber(luaState, vector.y);
	lua_pushnumber(luaState, vector.z);

	if (lua_pcall(luaState, 3, 1, 0) != 0)
	{
		string message = lua_tostring(luaState, -1);
		message = "Error running function: 'NewVector3'" + message;
		lua_pushstring(luaState, message.c_str());
		lua_error(luaState);
	}

	if (!lua_istable(luaState, -1))
	{
		lua_pushstring(luaState, "Wrong value passed back in function: 'NewVector3'");
		lua_error(luaState);
	}
	return 1;
}

int ScriptingManager::lua_SetRotation(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: SetRotation");
		lua_error(luaState);
	}

	ComponentPhysics * physicsComponent = (ComponentPhysics *)lua_touserdata(luaState, 1);

	if (!physicsComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: SetRotation");
		lua_error(luaState);
	}

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	physicsComponent->SetUpdateRotation(vec3(x, y, z));

	return 0;
}

int ScriptingManager::lua_GetDirection(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: GetDirection");
		lua_error(luaState);
	}

	ComponentDirection * directionComponent = (ComponentDirection *)lua_touserdata(luaState, 1);

	if (!directionComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetDirection");
		lua_error(luaState);
	}

	quat quaterion = directionComponent->GetUpdateDirection();

	lua_getglobal(luaState, "NewQuaterion");
	lua_pushnumber(luaState, quaterion.x);
	lua_pushnumber(luaState, quaterion.y);
	lua_pushnumber(luaState, quaterion.z);
	lua_pushnumber(luaState, quaterion.w);

	if (lua_pcall(luaState, 4, 1, 0) != 0)
	{
		string message = lua_tostring(luaState, -1);
		message = "Error running function: 'NewQuaterion'" + message;
		lua_pushstring(luaState, message.c_str());
		lua_error(luaState);
	}

	if (!lua_istable(luaState, -1))
	{
		lua_pushstring(luaState, "Wrong value passed back in function: 'NewQuaterion'");
		lua_error(luaState);
	}
	return 1;
}

int ScriptingManager::lua_SetDirection(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: SetDirection");
		lua_error(luaState);
	}

	ComponentDirection * directionComponent = (ComponentDirection *)lua_touserdata(luaState, 1);

	if (!directionComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: SetDirection");
		lua_error(luaState);
	}

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");
	lua_getfield(luaState, 2, "w");

	double x = lua_tonumber(luaState, -4);
	double y = lua_tonumber(luaState, -3);
	double z = lua_tonumber(luaState, -2);
	double w = lua_tonumber(luaState, -1);

	lua_pop(luaState, 4);

	directionComponent->SetUpdateDirection(quat(x, y, z, w));

	return 0;
}

int ScriptingManager::lua_GetTouchingGround(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: GetTouchingGround");
		lua_error(luaState);
	}

	ComponentPhysics * physicsComponent = (ComponentPhysics *)lua_touserdata(luaState, 1);

	if (!physicsComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetTouchingGround");
		lua_error(luaState);
	}

	bool touchingGround = physicsComponent->GetUpdateTouchingGround();

	lua_pushboolean(luaState, touchingGround);

	return 1;
}

int ScriptingManager::lua_CreateTranslationMatrix(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CreateTranslationMatrix");
		lua_error(luaState);
	}

	lua_getfield(luaState, 1, "x");
	lua_getfield(luaState, 1, "y");
	lua_getfield(luaState, 1, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	mat4 translationMatrix = translate(mat4(1), vec3(x, y, z));

	lua_getglobal(luaState, "NewMatrix4");

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			lua_pushnumber(luaState, translationMatrix[i][j]);
		}
	}

	if (lua_pcall(luaState, 16, 1, 0) != 0)
	{
		string message = lua_tostring(luaState, -1);
		message = "Error running function: 'NewMatrix4'" + message;
		lua_pushstring(luaState, message.c_str());
		lua_error(luaState);
	}

	if (!lua_istable(luaState, -1))
	{
		lua_pushstring(luaState, "Wrong value passed back in function: 'NewMatrix4'");
		lua_error(luaState);
	}
	return 1;
}

int ScriptingManager::lua_CreateScaleMatrix(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CreateScaleMatrix");
		lua_error(luaState);
	}

	lua_getfield(luaState, 1, "x");
	lua_getfield(luaState, 1, "y");
	lua_getfield(luaState, 1, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	mat4 scaleMatrix = scale(mat4(1), vec3(x, y, z));

	lua_getglobal(luaState, "NewMatrix4");

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			lua_pushnumber(luaState, scaleMatrix[i][j]);
		}
	}

	if (lua_pcall(luaState, 16, 1, 0) != 0)
	{
		string message = lua_tostring(luaState, -1);
		message = "Error running function: 'NewMatrix4'" + message;
		lua_pushstring(luaState, message.c_str());
		lua_error(luaState);
	}

	if (!lua_istable(luaState, -1))
	{
		lua_pushstring(luaState, "Wrong value passed back in function: 'NewMatrix4'");
		lua_error(luaState);
	}
	return 1;
}

int ScriptingManager::lua_CreateRotationMatrix(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CreateRotationMatrix");
		lua_error(luaState);
	}

	lua_getfield(luaState, 1, "x");
	lua_getfield(luaState, 1, "y");
	lua_getfield(luaState, 1, "z");
	lua_getfield(luaState, 1, "w");

	double x = lua_tonumber(luaState, -4);
	double y = lua_tonumber(luaState, -3);
	double z = lua_tonumber(luaState, -2);
	double w = lua_tonumber(luaState, -1);

	lua_pop(luaState, 4);

	mat4 rotationMatrix = mat4_cast(quat(x, y, z, w));

	lua_getglobal(luaState, "NewMatrix4");

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			lua_pushnumber(luaState, rotationMatrix[i][j]);
		}
	}

	if (lua_pcall(luaState, 16, 1, 0) != 0)
	{
		string message = lua_tostring(luaState, -1);
		message = "Error running function: 'NewMatrix4'" + message;
		lua_pushstring(luaState, message.c_str());
		lua_error(luaState);
	}

	if (!lua_istable(luaState, -1))
	{
		lua_pushstring(luaState, "Wrong value passed back in function: 'NewMatrix4'");
		lua_error(luaState);
	}
	return 1;
}

int ScriptingManager::lua_MultiplyMatrices(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: MultiplyMatrices");
		lua_error(luaState);
	}

	lua_getfield(luaState, 1, "x1");
	lua_getfield(luaState, 1, "y1");
	lua_getfield(luaState, 1, "z1");
	lua_getfield(luaState, 1, "w1");
	lua_getfield(luaState, 1, "x2");
	lua_getfield(luaState, 1, "y2");
	lua_getfield(luaState, 1, "z2");
	lua_getfield(luaState, 1, "w2");
	lua_getfield(luaState, 1, "x3");
	lua_getfield(luaState, 1, "y3");
	lua_getfield(luaState, 1, "z3");
	lua_getfield(luaState, 1, "w3");
	lua_getfield(luaState, 1, "x4");
	lua_getfield(luaState, 1, "y4");
	lua_getfield(luaState, 1, "z4");
	lua_getfield(luaState, 1, "w4");

	double x1 = lua_tonumber(luaState, -16);
	double y1 = lua_tonumber(luaState, -15);
	double z1 = lua_tonumber(luaState, -14);
	double w1 = lua_tonumber(luaState, -13);
	double x2 = lua_tonumber(luaState, -12);
	double y2 = lua_tonumber(luaState, -11);
	double z2 = lua_tonumber(luaState, -10);
	double w2 = lua_tonumber(luaState, -9);
	double x3 = lua_tonumber(luaState, -8);
	double y3 = lua_tonumber(luaState, -7);
	double z3 = lua_tonumber(luaState, -6);
	double w3 = lua_tonumber(luaState, -5);
	double x4 = lua_tonumber(luaState, -4);
	double y4 = lua_tonumber(luaState, -3);
	double z4 = lua_tonumber(luaState, -2);
	double w4 = lua_tonumber(luaState, -1);

	lua_pop(luaState, 16);

	mat4 matrix1 = mat4(x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4);

	lua_getfield(luaState, 2, "x1");
	lua_getfield(luaState, 2, "y1");
	lua_getfield(luaState, 2, "z1");
	lua_getfield(luaState, 2, "w1");
	lua_getfield(luaState, 2, "x2");
	lua_getfield(luaState, 2, "y2");
	lua_getfield(luaState, 2, "z2");
	lua_getfield(luaState, 2, "w2");
	lua_getfield(luaState, 2, "x3");
	lua_getfield(luaState, 2, "y3");
	lua_getfield(luaState, 2, "z3");
	lua_getfield(luaState, 2, "w3");
	lua_getfield(luaState, 2, "x4");
	lua_getfield(luaState, 2, "y4");
	lua_getfield(luaState, 2, "z4");
	lua_getfield(luaState, 2, "w4");

	x1 = lua_tonumber(luaState, -16);
	y1 = lua_tonumber(luaState, -15);
	z1 = lua_tonumber(luaState, -14);
	w1 = lua_tonumber(luaState, -13);
	x2 = lua_tonumber(luaState, -12);
	y2 = lua_tonumber(luaState, -11);
	z2 = lua_tonumber(luaState, -10);
	w2 = lua_tonumber(luaState, -9);
	x3 = lua_tonumber(luaState, -8);
	y3 = lua_tonumber(luaState, -7);
	z3 = lua_tonumber(luaState, -6);
	w3 = lua_tonumber(luaState, -5);
	x4 = lua_tonumber(luaState, -4);
	y4 = lua_tonumber(luaState, -3);
	z4 = lua_tonumber(luaState, -2);
	w4 = lua_tonumber(luaState, -1);

	lua_pop(luaState, 16);

	mat4 matrix2 = mat4(x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4);

	mat4 matrix3 = matrix1 * matrix2;

	lua_getglobal(luaState, "NewMatrix4");

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			lua_pushnumber(luaState, matrix3[i][j]);
		}
	}

	if (lua_pcall(luaState, 16, 1, 0) != 0)
	{
		string message = lua_tostring(luaState, -1);
		message = "Error running function: 'NewMatrix4'" + message;
		lua_pushstring(luaState, message.c_str());
		lua_error(luaState);
	}

	if (!lua_istable(luaState, -1))
	{
		lua_pushstring(luaState, "Wrong value passed back in function: 'NewMatrix4'");
		lua_error(luaState);
	}
	return 1;
}

int ScriptingManager::lua_MultiplyMatrixVector(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: MultiplyMatrixVector");
		lua_error(luaState);
	}

	lua_getfield(luaState, 1, "x1");
	lua_getfield(luaState, 1, "y1");
	lua_getfield(luaState, 1, "z1");
	lua_getfield(luaState, 1, "w1");
	lua_getfield(luaState, 1, "x2");
	lua_getfield(luaState, 1, "y2");
	lua_getfield(luaState, 1, "z2");
	lua_getfield(luaState, 1, "w2");
	lua_getfield(luaState, 1, "x3");
	lua_getfield(luaState, 1, "y3");
	lua_getfield(luaState, 1, "z3");
	lua_getfield(luaState, 1, "w3");
	lua_getfield(luaState, 1, "x4");
	lua_getfield(luaState, 1, "y4");
	lua_getfield(luaState, 1, "z4");
	lua_getfield(luaState, 1, "w4");

	double x1 = lua_tonumber(luaState, -16);
	double y1 = lua_tonumber(luaState, -15);
	double z1 = lua_tonumber(luaState, -14);
	double w1 = lua_tonumber(luaState, -13);
	double x2 = lua_tonumber(luaState, -12);
	double y2 = lua_tonumber(luaState, -11);
	double z2 = lua_tonumber(luaState, -10);
	double w2 = lua_tonumber(luaState, -9);
	double x3 = lua_tonumber(luaState, -8);
	double y3 = lua_tonumber(luaState, -7);
	double z3 = lua_tonumber(luaState, -6);
	double w3 = lua_tonumber(luaState, -5);
	double x4 = lua_tonumber(luaState, -4);
	double y4 = lua_tonumber(luaState, -3);
	double z4 = lua_tonumber(luaState, -2);
	double w4 = lua_tonumber(luaState, -1);

	lua_pop(luaState, 16);

	mat4 matrix = mat4(x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4);

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	vec4 vector1 = vec4(x, y, z, 1);

	vec3 vector2 = vector1 * matrix;

	lua_getglobal(luaState, "NewVector3");
	lua_pushnumber(luaState, vector2.x);
	lua_pushnumber(luaState, vector2.y);
	lua_pushnumber(luaState, vector2.z);

	if (lua_pcall(luaState, 3, 1, 0) != 0)
	{
		string message = lua_tostring(luaState, -1);
		message = "error running function: 'NewVector3'" + message;
		lua_pushstring(luaState, message.c_str());
		lua_error(luaState);
	}

	if (!lua_istable(luaState, -1))
	{
		lua_pushstring(luaState, "Wrong value passed back in function: 'NewVector3'");
		lua_error(luaState);
	}
	return 1;
}

int ScriptingManager::lua_DeleteEntity(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: DeleteEntity");
		lua_error(luaState);
	}

	Entity * entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: DeleteEntity");
		lua_error(luaState);
	}

	EntityManager::Instance()->AddToDeleteList(entity);

	return 0;
}

void ScriptingManager::RunScriptFromCollision(string & function, Entity * entity)
{
	lua_getglobal(luaVM, function.c_str());
	lua_pushlightuserdata(luaVM, entity);

	if (lua_pcall(luaVM, 1, 0, 0) != 0)
	{
		string message = lua_tostring(luaVM, -1);
		message = "Error running lua script: " + message;
		LoggingManager::LogMessage(LOG, message);
	}
}

void ScriptingManager::RunScriptFromInput(string & function, Entity * entity, float inputValue, float deltaTime)
{
	lua_getglobal(luaVM, function.c_str());
	lua_pushlightuserdata(luaVM, entity);
	lua_pushnumber(luaVM, inputValue);
	lua_pushnumber(luaVM, deltaTime);

	if (lua_pcall(luaVM, 3, 0, 0) != 0)
	{
		string message = lua_tostring(luaVM, -1);
		message = "Error running lua script: " + message;
		LoggingManager::LogMessage(LOG, message);
	}
}

ScriptingManager * ScriptingManager::instance = nullptr;
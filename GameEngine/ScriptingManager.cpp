#include "ScriptingManager.h"
#include "LoggingManager.h"
#include "Entity.h"
#include "EntityManager.h"
#include "InputFunction.h"
#include "CollisionCuboid.h"
#include "CollisionSphere.h"
#include "ComponentAnimation.h"
#include "ComponentState.h"
#include "ComponentArtificalIntelligence.h"
#include "ComponentRiggedAnimation.h"
#include "ComponentDirection.h"
#include "ComponentPosition.h"
#include "ComponentInput.h"
#include "ComponentPhysics.h"
#include "ComponentModel.h"
#include "ComponentNormalTexture.h"
#include "ComponentTexture.h"
#include "ComponentAudio.h"
#include "ComponentShader.h"
#include "ComponentShadowShader.h"
#include "AudioManager.h"
#include "PhysicsManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SceneManager.h"
#include "MenuScene.h"
#include "GameScene.h"
#include <sstream>

extern "C"
{
	#include "Lua/lua.h"
	#include "Lua/lauxlib.h"
	#include "Lua/lualib.h"
}

ScriptingManager::ScriptingManager() : luaVM(luaL_newstate())
{
	if (luaVM == nullptr)
	{
		const std::string message = "Failed to Initialize lua";
		LoggingManager::LogMessage(MESSAGE_TYPE::SEVERE, message);
	}

	luaL_openlibs(luaVM);

	lua_register(luaVM, "GetComponentPhysics", lua_GetComponentPhysics);
	lua_register(luaVM, "GetComponentPosition", lua_GetComponentPosition);
	lua_register(luaVM, "GetComponentDirection", lua_GetComponentDirection);
	lua_register(luaVM, "GetComponentAudio", lua_GetComponentAudio);
	lua_register(luaVM, "GetComponentState", lua_GetComponentState);
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
	lua_register(luaVM, "ChangePlayback", lua_ChangePlayback);
	lua_register(luaVM, "PlayAudio", lua_PlayAudio);
	lua_register(luaVM, "PlayAudioAtLocation", lua_PlayAudioAtLocation);
	lua_register(luaVM, "PlayAudioAtEntityLocation", lua_PlayAudioAtEntityLocation);
	lua_register(luaVM, "DeleteEntity", lua_DeleteEntity);
	lua_register(luaVM, "GetAIValue", lua_GetAIValue);
	lua_register(luaVM, "SetAIValue", lua_SetAIValue);
	lua_register(luaVM, "GetValue", lua_GetValue);
	lua_register(luaVM, "SetValue", lua_SetValue);
	lua_register(luaVM, "CanSeePlayer", lua_CanSeePlayer);
	lua_register(luaVM, "MoveOffPath", lua_MoveOffPath);
	lua_register(luaVM, "OnPath", lua_OnPath);
	lua_register(luaVM, "FindAIPath", lua_FindAIPath);
	lua_register(luaVM, "FindPath", lua_FindPath);
	lua_register(luaVM, "GetNearestPath", lua_GetNearestPath);
	lua_register(luaVM, "NewGameScene", lua_NewGameScene);
	lua_register(luaVM, "NewMenuScene", lua_NewMenuScene);
	lua_register(luaVM, "SwapToMenuScene", lua_SwapToMenuScene);
	lua_register(luaVM, "SwapToGameScene", lua_SwapToGameScene);
	lua_register(luaVM, "CloseScene", lua_CloseScene);
	lua_register(luaVM, "CloseWindow", lua_CloseWindow);

	lua_register(luaVM, "CreateEntity", lua_CreateEntity);
	lua_register(luaVM, "GetEntity", lua_GetEntity);
	lua_register(luaVM, "AddComponentAnimation", lua_AddComponentAnimation);
	lua_register(luaVM, "CreateAIStateMachine", lua_CreateAIStateMachine);
	lua_register(luaVM, "CreatePathFinder", lua_CreatePathFinder);
	lua_register(luaVM, "CreatePathVector", lua_CreatePathVector);
	lua_register(luaVM, "AddToPathVector", lua_AddToPathVector);
	lua_register(luaVM, "CreatePathFollower", lua_CreatePathFollower);
	lua_register(luaVM, "AddComponentArtificialIntelligence", lua_AddComponentArtificalIntelligence);
	lua_register(luaVM, "AddComponentAudio", lua_AddComponentAudio);
	lua_register(luaVM, "AddComponentDirection", lua_AddComponentDirection);
	lua_register(luaVM, "CreateInputFunction", lua_CreateInputFunction);
	lua_register(luaVM, "AddInputToInputFunction", lua_AddInputToInputFunction);
	lua_register(luaVM, "CreateInputVector", lua_CreateInputVector);
	lua_register(luaVM, "AddToInputVector", lua_AddToInputVector);
	lua_register(luaVM, "AddComponentInput", lua_AddComponentInput);
	lua_register(luaVM, "AddComponentModel", lua_AddComponentModel);
	lua_register(luaVM, "AddComponentNormalTexture", lua_AddComponentNormalTexture);
	lua_register(luaVM, "CreateCollisionSphere", lua_CreateCollisionSphere);
	lua_register(luaVM, "CreateCollisionCuboid", lua_CreateCollisionCuboid);
	lua_register(luaVM, "CreateCollisionFunctionMap", lua_CreateCollisionFunctionMap);
	lua_register(luaVM, "AddToCollisionFunctionMap", lua_AddToCollisionFunctionMap);
	lua_register(luaVM, "AddComponentPhysics", lua_AddComponentPhysics);
	lua_register(luaVM, "AddComponentPosition", lua_AddComponentPosition);
	lua_register(luaVM, "AddComponentRiggedAnimation", lua_AddComponentRiggedAnimation);
	lua_register(luaVM, "AddComponentShader", lua_AddComponentShader);
	lua_register(luaVM, "AddComponentShadowShader", lua_AddComponentShadowShader);
	lua_register(luaVM, "AddComponentState", lua_AddComponentState);
	lua_register(luaVM, "AddComponentTexture", lua_AddComponentTexture);
	lua_register(luaVM, "FinishEntity", lua_FinishEntity);

	lua_register(luaVM, "DisplayText", lua_DisplayText);
	
	std::string file = "Scripts/Vector3.lua";
	LoadLuaFromFile(file);

	file = "Scripts/Matrix4.lua";
	LoadLuaFromFile(file);

	file = "Scripts/Quaternion.lua";
	LoadLuaFromFile(file);
}

void ScriptingManager::LoadLuaFromFile(const std::string & file) const
{
	int iStatus = luaL_loadfile(luaVM, file.c_str());
	if (iStatus)
	{
		std::string message = lua_tostring(luaVM, -1);
		message = "Error: " + message;
		LoggingManager::LogMessage(MESSAGE_TYPE::SEVERE, message);
	}

	if (lua_pcall(luaVM, 0, 0, 0))
	{
		std::string message = lua_tostring(luaVM, -1);
		message = "Error: " + message;
		LoggingManager::LogMessage(MESSAGE_TYPE::SEVERE, message);
	}
}

ScriptingManager::~ScriptingManager()
{
	lua_close(luaVM);
}

int ScriptingManager::lua_GetComponentDirection(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: GetComponentDirection");
		lua_error(luaState);
	}

	Entity * const entity = static_cast<Entity *>(lua_touserdata(luaState, 1));

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetComponentDirection");
		lua_error(luaState);
	}

	iComponent * const componentDirection = EntityManager::Instance()->GetComponentOfEntity(entity, ComponentType::COMPONENT_DIRECTION);
	ComponentDirection * const directionComponent = dynamic_cast<ComponentDirection *>(componentDirection);

	lua_pushlightuserdata(luaState, directionComponent);
	return 1;
}

int ScriptingManager::lua_GetComponentPosition(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: GetComponentPosition");
		lua_error(luaState);
	}

	Entity * const entity = static_cast<Entity *>(lua_touserdata(luaState, 1));

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetComponentPosition");
		lua_error(luaState);
	}

	iComponent * const componentPosition = EntityManager::Instance()->GetComponentOfEntity(entity, ComponentType::COMPONENT_POSITION);
	ComponentPosition * const positionComponent = dynamic_cast<ComponentPosition *>(componentPosition);

	lua_pushlightuserdata(luaState, positionComponent);
	return 1;
}

int ScriptingManager::lua_GetComponentPhysics(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: GetComponentPhysics");
		lua_error(luaState);
	}

	Entity * const entity = static_cast<Entity *>(lua_touserdata(luaState, 1));

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetComponentPhysics");
		lua_error(luaState);
	}

	iComponent * const componentPhysics = EntityManager::Instance()->GetComponentOfEntity(entity, ComponentType::COMPONENT_PHYSICS);
	ComponentPhysics * const physicsComponent = dynamic_cast<ComponentPhysics *>(componentPhysics);

	lua_pushlightuserdata(luaState, physicsComponent);
	return 1;
}

int ScriptingManager::lua_GetComponentAudio(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: GetComponentAudio");
		lua_error(luaState);
	}

	Entity * const entity = static_cast<Entity *>(lua_touserdata(luaState, 1));

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetComponentAudio");
		lua_error(luaState);
	}

	iComponent * const componentAudio = EntityManager::Instance()->GetComponentOfEntity(entity, ComponentType::COMPONENT_AUDIO);
	ComponentAudio * const audioComponent = dynamic_cast<ComponentAudio *>(componentAudio);

	lua_pushlightuserdata(luaState, audioComponent);
	return 1;
}

int ScriptingManager::lua_GetComponentState(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: GetComponentState");
		lua_error(luaState);
	}

	Entity * const entity = static_cast<Entity *>(lua_touserdata(luaState, 1));

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetComponentState");
		lua_error(luaState);
	}

	iComponent * const componentState = EntityManager::Instance()->GetComponentOfEntity(entity, ComponentType::COMPONENT_STATE);
	ComponentState * const stateComponent = dynamic_cast<ComponentState *>(componentState);

	lua_pushlightuserdata(luaState, stateComponent);
	return 1;
}

int ScriptingManager::lua_GetPosition(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: GetPosition");
		lua_error(luaState);
	}
	ComponentPosition * const positionComponent = static_cast<ComponentPosition *>(lua_touserdata(luaState, 1));

	if (!positionComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetPosition");
		lua_error(luaState);
	}

	const glm::vec3 vector = positionComponent->GetUpdatePosition();

	lua_getglobal(luaState, "NewVector3");
	lua_pushnumber(luaState, vector.x);
	lua_pushnumber(luaState, vector.y);
	lua_pushnumber(luaState, vector.z);

	if (lua_pcall(luaState, 3, 1, 0) != 0)
	{
		std::string message = lua_tostring(luaState, -1);
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

int ScriptingManager::lua_SetPosition(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: SetPosition");
		lua_error(luaState);
	}

	ComponentPosition * const positionComponent = static_cast<ComponentPosition *>(lua_touserdata(luaState, 1));

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

	const glm::vec3 position(x, y, z);
	positionComponent->SetUpdatePosition(position);

	return 0;
}

int ScriptingManager::lua_GetVelocity(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: GetVelocity");
		lua_error(luaState);
	}

	ComponentPhysics * const physicsComponent = static_cast<ComponentPhysics *>(lua_touserdata(luaState, 1));

	if (!physicsComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetVelocity");
		lua_error(luaState);
	}

	const glm::vec3 vector = physicsComponent->GetUpdateVelocity();

	lua_getglobal(luaState, "NewVector3");
	lua_pushnumber(luaState, vector.x);
	lua_pushnumber(luaState, vector.y);
	lua_pushnumber(luaState, vector.z);

	if (lua_pcall(luaState, 3, 1, 0) != 0)
	{
		std::string message = lua_tostring(luaState, -1);
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

int ScriptingManager::lua_SetVelocity(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: SetVelocity");
		lua_error(luaState);
	}

	ComponentPhysics * const physicsComponent = static_cast<ComponentPhysics *>(lua_touserdata(luaState, 1));

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

	const glm::vec3 velocity(x, y, z);
	physicsComponent->SetUpdateVelocity(velocity);

	return 0;
}

int ScriptingManager::lua_GetImpulse(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: GetImpluse");
		lua_error(luaState);
	}

	ComponentPhysics * const physicsComponent = static_cast<ComponentPhysics *>(lua_touserdata(luaState, 1));

	if (!physicsComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetImpluse");
		lua_error(luaState);
	}

	const glm::vec3 vector = physicsComponent->GetUpdateImpulse();

	lua_getglobal(luaState, "NewVector3");
	lua_pushnumber(luaState, vector.x);
	lua_pushnumber(luaState, vector.y);
	lua_pushnumber(luaState, vector.z);

	if (lua_pcall(luaState, 3, 1, 0) != 0)
	{
		std::string message = lua_tostring(luaState, -1);
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

int ScriptingManager::lua_SetImpulse(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: SetImpluse");
		lua_error(luaState);
	}

	ComponentPhysics * const physicsComponent = static_cast<ComponentPhysics *>(lua_touserdata(luaState, 1));

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

	const glm::vec3 impulse(x, y, z);
	physicsComponent->SetUpdateImpulse(impulse);

	return 0;
}

int ScriptingManager::lua_GetRotation(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: GetRotation");
		lua_error(luaState);
	}

	ComponentPhysics * const physicsComponent = static_cast<ComponentPhysics *>(lua_touserdata(luaState, 1));

	if (!physicsComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetRotation");
		lua_error(luaState);
	}

	const glm::vec3 vector = physicsComponent->GetUpdateRotation();

	lua_getglobal(luaState, "NewVector3");
	lua_pushnumber(luaState, vector.x);
	lua_pushnumber(luaState, vector.y);
	lua_pushnumber(luaState, vector.z);

	if (lua_pcall(luaState, 3, 1, 0) != 0)
	{
		std::string message = lua_tostring(luaState, -1);
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

int ScriptingManager::lua_SetRotation(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: SetRotation");
		lua_error(luaState);
	}

	ComponentPhysics * physicsComponent = static_cast<ComponentPhysics *>(lua_touserdata(luaState, 1));

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

	const glm::vec3 rotation(x, y, z);
	physicsComponent->SetUpdateRotation(rotation);

	return 0;
}

int ScriptingManager::lua_GetDirection(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: GetDirection");
		lua_error(luaState);
	}

	ComponentDirection * const directionComponent = static_cast<ComponentDirection *>(lua_touserdata(luaState, 1));

	if (!directionComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetDirection");
		lua_error(luaState);
	}

	const glm::quat quaterion = directionComponent->GetUpdateDirection();

	lua_getglobal(luaState, "NewQuaternion");
	lua_pushnumber(luaState, quaterion.x);
	lua_pushnumber(luaState, quaterion.y);
	lua_pushnumber(luaState, quaterion.z);
	lua_pushnumber(luaState, quaterion.w);

	if (lua_pcall(luaState, 4, 1, 0) != 0)
	{
		std::string message = lua_tostring(luaState, -1);
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

int ScriptingManager::lua_SetDirection(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: SetDirection");
		lua_error(luaState);
	}

	ComponentDirection * const directionComponent = static_cast<ComponentDirection *>(lua_touserdata(luaState, 1));

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

	directionComponent->SetUpdateDirection(glm::quat(x, y, z, w));

	return 0;
}

int ScriptingManager::lua_GetTouchingGround(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: GetTouchingGround");
		lua_error(luaState);
	}

	ComponentPhysics * const physicsComponent = static_cast<ComponentPhysics *>(lua_touserdata(luaState, 1));

	if (!physicsComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetTouchingGround");
		lua_error(luaState);
	}

	const bool touchingGround = physicsComponent->GetUpdateTouchingGround();

	lua_pushboolean(luaState, touchingGround);

	return 1;
}

int ScriptingManager::lua_CreateTranslationMatrix(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

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

	glm::mat4 translationMatrix = glm::translate(glm::mat4(1), glm::vec3(x, y, z));

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
		std::string message = lua_tostring(luaState, -1);
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

int ScriptingManager::lua_CreateScaleMatrix(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

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

	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(x, y, z));

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
		std::string message = lua_tostring(luaState, -1);
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

int ScriptingManager::lua_CreateRotationMatrix(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

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

	glm::mat4 rotationMatrix = glm::mat4_cast(glm::quat(x, y, z, w));

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
		std::string message = lua_tostring(luaState, -1);
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

int ScriptingManager::lua_MultiplyMatrices(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

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

	glm::mat4 matrix1 = glm::mat4(x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4);

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

	glm::mat4 matrix2 = glm::mat4(x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4);

	glm::mat4 matrix3 = matrix1 * matrix2;

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
		std::string message = lua_tostring(luaState, -1);
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

int ScriptingManager::lua_MultiplyMatrixVector(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

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

	const glm::mat4 matrix = glm::mat4(x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4);

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	const glm::vec4 vector1 = glm::vec4(x, y, z, 1);

	const glm::vec3 vector2 = vector1 * matrix;

	lua_getglobal(luaState, "NewVector3");
	lua_pushnumber(luaState, vector2.x);
	lua_pushnumber(luaState, vector2.y);
	lua_pushnumber(luaState, vector2.z);

	if (lua_pcall(luaState, 3, 1, 0) != 0)
	{
		std::string message = lua_tostring(luaState, -1);
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

int ScriptingManager::lua_ChangePlayback(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: ChangePlayback");
		lua_error(luaState);
	}

	ComponentAudio * const audioComponent = static_cast<ComponentAudio *>(lua_touserdata(luaState, 1));

	if (!audioComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: ChangePlayback");
		lua_error(luaState);
	}

	std::string playback = lua_tostring(luaState, 2);

	if (playback == "Play")
	{
		audioComponent->SetUpdateAudioPlayback(AudioPlayback::PLAY);
	}
	else if (playback == "Pause")
	{
		audioComponent->SetUpdateAudioPlayback(AudioPlayback::PAUSE);
	}
	else if (playback == "Stop")
	{
		audioComponent->SetUpdateAudioPlayback(AudioPlayback::STOP);
	}
	else
	{
		lua_pushstring(luaState, "Wrong playback state passed in: ChangePlayback");
	}

	return 0;
}

int ScriptingManager::lua_PlayAudio(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number of Args: PlayAudio");
		lua_error(luaState);
	}

	std::string sound = lua_tostring(luaState, 1);

	AudioManager::Instance()->PlayAudio(sound);

	return 0;
}

int ScriptingManager::lua_PlayAudioAtLocation(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: PlayAudioAtLocation");
		lua_error(luaState);
	}

	std::string sound = lua_tostring(luaState, 1);

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	const glm::vec3 position(x, y, z);
	
	AudioManager::Instance()->PlayAudioAtLocation(sound, position);

	return 0;
}

int ScriptingManager::lua_PlayAudioAtEntityLocation(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: PlayAudioAtEntityLocation");
		lua_error(luaState);
	}

	std::string sound = lua_tostring(luaState, 1);

	Entity * const entity = static_cast<Entity *>(lua_touserdata(luaState, 2));

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: PlayAudioAtEntityLocation");
		lua_error(luaState);
	}

	AudioManager::Instance()->PlayAudioAtEntityLocation(sound, entity);

	return 0;
}

int ScriptingManager::lua_DeleteEntity(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: DeleteEntity");
		lua_error(luaState);
	}

	Entity * const entity = static_cast<Entity *>(lua_touserdata(luaState, 1));

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: DeleteEntity");
		lua_error(luaState);
	}

	EntityManager::Instance()->AddToDeleteList(entity);

	return 0;
}

int ScriptingManager::lua_GetAIValue(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 4)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: GetAIValue");
		lua_error(luaState);
	}

	AIStateMachine * const stateMachine = static_cast<AIStateMachine *>(lua_touserdata(luaState, 1));

	if (!stateMachine)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetAIValue");
		lua_error(luaState);
	}

	std::string valueName = lua_tostring(luaState, 2);
	std::string valueType = lua_tostring(luaState, 3);
	std::string defaultValue = lua_tostring(luaState, 4);

	std::string valueString = stateMachine->GetValue(valueName, defaultValue);

	std::istringstream iString(valueString);

	if (valueType == "integer")
	{
		int number;
		iString >> number;

		lua_pushinteger(luaState, number);
	}
	else if (valueType == "float")
	{
		float number;
		iString >> number;

		lua_pushnumber(luaState, number);
	}
	else if (valueType == "boolean")
	{
		bool type;
		iString >> type;

		lua_pushboolean(luaState, type);
	}
	else if (valueType == "string")
	{
		lua_pushstring(luaState, valueString.c_str());
	}
	else
	{
		lua_pushstring(luaState, "Value Type Not Recognised: GetAIValue");
		lua_error(luaState);
	}

	return 1;
}

int ScriptingManager::lua_SetAIValue(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 3)
	{
		lua_pushstring(luaState, "Wrong Number of Args: SetAIValue");
		lua_error(luaState);
	}

	AIStateMachine * const stateMachine = static_cast<AIStateMachine *>(lua_touserdata(luaState, 1));

	if (!stateMachine)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: SetAIValue");
		lua_error(luaState);
	}
	
	std::string valueName = lua_tostring(luaState, 2);
	std::string value;

	std::ostringstream oString;

	if (lua_isboolean(luaState, 3))
	{
		const bool preValue = lua_toboolean(luaState, 3);
		oString << preValue;
		value = oString.str();
	}
	else if (lua_isnumber(luaState, 3))
	{
		float preValue = lua_tonumber(luaState, 3);
		oString << preValue;
		value = oString.str();
	}
	else if (lua_isinteger(luaState, 3))
	{
		int preValue = lua_tointeger(luaState, 3);
		oString << preValue;
		value = oString.str();
	}
	else
	{
		value = lua_tostring(luaState, 3);
	}

	stateMachine->SetValue(valueName, value);
	
	return 0;
}

int ScriptingManager::lua_GetValue(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 4)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: GetValue");
		lua_error(luaState);
	}

	ComponentState * const stateComponent = static_cast<ComponentState *>(lua_touserdata(luaState, 1));

	if (!stateComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetValue");
		lua_error(luaState);
	}

	std::string valueName = lua_tostring(luaState, 2);
	std::string valueType = lua_tostring(luaState, 3);
	std::string defaultValue = lua_tostring(luaState, 4);

	std::string valueString = stateComponent->GetValue(valueName, defaultValue);

	std::istringstream iString(valueString);

	if (valueType == "integer")
	{
		int number;
		iString >> number;

		lua_pushinteger(luaState, number);
	}
	else if (valueType == "float")
	{
		float number;
		iString >> number;

		lua_pushnumber(luaState, number);
	}
	else if (valueType == "boolean")
	{
		bool type;
		iString >> type;
		
		lua_pushboolean(luaState, type);
	}
	else if (valueType == "string")
	{
		lua_pushstring(luaState, valueString.c_str());
	}
	else
	{
		lua_pushstring(luaState, "Value Type Not Recognised: GetValue");
		lua_error(luaState);
	}

	return 1;
}

int ScriptingManager::lua_SetValue(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 3)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: SetValue");
		lua_error(luaState);
	}

	ComponentState * const stateComponent = static_cast<ComponentState *>(lua_touserdata(luaState, 1));

	if (!stateComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: SetValue");
		lua_error(luaState);
	}

	std::string valueName = lua_tostring(luaState, 2);
	std::string value;

	std::ostringstream oString;

	if (lua_isboolean(luaState, 3))
	{
		const bool preValue = lua_toboolean(luaState, 3);
		oString << preValue;
		value = oString.str();
	}
	else if (lua_isnumber(luaState, 3))
	{
		float preValue = lua_tonumber(luaState, 3);
		oString << preValue;
		value = oString.str();
	}
	else if (lua_isinteger(luaState, 3))
	{
		int preValue = lua_tointeger(luaState, 3);
		oString << preValue;
		value = oString.str();
	}
	else
	{
		value = lua_tostring(luaState, 3);
	}

	stateComponent->SetValue(valueName, value);

	return 0;
}

int ScriptingManager::lua_CanSeePlayer(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CanSeePlayer");
		lua_error(luaState);
	}

	lua_getfield(luaState, 1, "x");
	lua_getfield(luaState, 1, "y");
	lua_getfield(luaState, 1, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	const glm::vec3 guardPosition(x, y, z);

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	x = lua_tonumber(luaState, -3);
	y = lua_tonumber(luaState, -2);
	z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	const glm::vec3 playerPosition(x, y, z);

	const bool clear = PhysicsManager::Instance()->ClearBetweenPoints(guardPosition, playerPosition);

	lua_pushboolean(luaState, clear);

	return 1;
}

int ScriptingManager::lua_MoveOffPath(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: MoveOffPath");
		lua_error(luaState);
	}

	AIStateMachine * const stateMachine = static_cast<AIStateMachine *>(lua_touserdata(luaState, 1));

	if (!stateMachine)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: MoveOffPath");
		lua_error(luaState);
	}

	stateMachine->MoveOffPath();

	return 0;
}

int ScriptingManager::lua_OnPath(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: OnPath");
		lua_error(luaState);
	}

	AIStateMachine * const stateMachine = static_cast<AIStateMachine *>(lua_touserdata(luaState, 1));

	if (!stateMachine)
	{
		lua_pushstring(luaState, "Wrong Parameters Passedd in: OnPath");
		lua_error(luaState);
	}

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	const glm::vec3 position(x, y, z);

	const bool onPath = stateMachine->OnPath(position);

	lua_pushboolean(luaState, onPath);

	return 1;
}

int ScriptingManager::lua_FindAIPath(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 5 && numberOfArgs != 6)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: FindAIPath");
		lua_error(luaState);
	}

	AIStateMachine * const stateMachine = static_cast<AIStateMachine *>(lua_touserdata(luaState, 1));

	if (!stateMachine)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: FindAIPath");
		lua_error(luaState);
	}

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	const glm::vec3 position(x, y, z);

	lua_getfield(luaState, 3, "x");
	lua_getfield(luaState, 3, "y");
	lua_getfield(luaState, 3, "z");
	lua_getfield(luaState, 3, "w");

	x = lua_tonumber(luaState, -4);
	y = lua_tonumber(luaState, -3);
	z = lua_tonumber(luaState, -2);
	double w = lua_tonumber(luaState, -1);

	lua_pop(luaState, 4);

	const glm::quat direction(w, x, y, z);

	ComponentPhysics * const physicsComponent = static_cast<ComponentPhysics *>(lua_touserdata(luaState, 4));

	if (!physicsComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in FindAIPath");
		lua_error(luaState);
	}

	if (numberOfArgs == 5)
	{
		float deltaTime = lua_tonumber(luaState, 5);

		stateMachine->FindAIPath(position, direction, physicsComponent, deltaTime);
	}
	else
	{
		lua_getfield(luaState, 5, "x");
		lua_getfield(luaState, 5, "y");
		lua_getfield(luaState, 5, "z");


		x = lua_tonumber(luaState, -3);
		y = lua_tonumber(luaState, -2);
		z = lua_tonumber(luaState, -1);

		lua_pop(luaState, 3);

		const glm::vec3 nearestPoint(x, y, z);

		float deltaTime = lua_tonumber(luaState, 6);

		stateMachine->FindAIPath(position, direction, physicsComponent, nearestPoint, deltaTime);
	}

	return 0;
}

int ScriptingManager::lua_FindPath(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 5)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: FindPath");
		lua_error(luaState);
	}

	AIStateMachine * const stateMachine = static_cast<AIStateMachine *>(lua_touserdata(luaState, 1));

	if (!stateMachine)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: FindPath");
		lua_error(luaState);
	}

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	const glm::vec3 position(x, y, z);

	lua_getfield(luaState, 3, "x");
	lua_getfield(luaState, 3, "y");
	lua_getfield(luaState, 3, "z");
	lua_getfield(luaState, 3, "w");

	x = lua_tonumber(luaState, -4);
	y = lua_tonumber(luaState, -3);
	z = lua_tonumber(luaState, -2);
	double w = lua_tonumber(luaState, -1);

	lua_pop(luaState, 4);

	const glm::quat direction(w, x, y, z);

	ComponentPhysics * const physicsComponent = static_cast<ComponentPhysics *>(lua_touserdata(luaState, 4));

	if (!physicsComponent)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in FindPath");
		lua_error(luaState);
	}

	float deltaTime = lua_tonumber(luaState, 5);

	stateMachine->FindPath(position, direction, physicsComponent, deltaTime);

	return 0;
}

int ScriptingManager::lua_GetNearestPath(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: GetNearestPath");
		lua_error(luaState);
	}

	AIStateMachine * const stateMachine = static_cast<AIStateMachine *>(lua_touserdata(luaState, 1));

	if (!stateMachine)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: GetNearestPath");
		lua_error(luaState);
	}

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	const glm::vec3 position(x, y, z);

	const glm::vec3 nearestPath = stateMachine->GetNearestPath(position);

	lua_getglobal(luaState, "NewVector3");
	lua_pushnumber(luaState, nearestPath.x);
	lua_pushnumber(luaState, nearestPath.y);
	lua_pushnumber(luaState, nearestPath.z);

	if (lua_pcall(luaState, 3, 1, 0) != 0)
	{
		std::string message = lua_tostring(luaState, -1);
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

int ScriptingManager::lua_NewGameScene(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: NewGameScene");
		lua_error(luaState);
	}

	std::string scene = lua_tostring(luaState, 1);

	SceneManager::Instance()->NewScene(new GameScene(scene));

	return 0;
}

int ScriptingManager::lua_NewMenuScene(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: NewMenuScene");
		lua_error(luaState);
	}

	std::string scene = lua_tostring(luaState, 1);

	SceneManager::Instance()->NewScene(new MenuScene(scene));

	return 0;
}

int ScriptingManager::lua_SwapToMenuScene(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: SwapToMenuScene");
		lua_error(luaState);
	}

	std::string scene = lua_tostring(luaState, 1);

	SceneManager::Instance()->SwapScene(new MenuScene(scene));

	return 0;
}

int ScriptingManager::lua_SwapToGameScene(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: SwapToGameScene");
		lua_error(luaState);
	}

	std::string scene = lua_tostring(luaState, 1);

	SceneManager::Instance()->SwapScene(new GameScene(scene));

	return 0;
}

int ScriptingManager::lua_CloseScene(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs == 0)
	{
		SceneManager::Instance()->CloseScene();
	}
	else if (numberOfArgs == 1)
	{
		int noClose = lua_tointeger(luaState, 1);
		SceneManager::Instance()->CloseScene(noClose);
	}
	else
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CloseScene");
		lua_error(luaState);
	}

	return 0;
}

int ScriptingManager::lua_CloseWindow(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 0)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CloseWindow");
		lua_error(luaState);
	}

	SceneManager::Instance()->CloseWindow();

	return 0;
}

int ScriptingManager::lua_CreateEntity(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs == 1)
	{
		std::string entityName = lua_tostring(luaState, 1);
		Entity * const entity = EntityManager::Instance()->CreateEntity(entityName);
		lua_pushlightuserdata(luaState, entity);
	}
	else if (numberOfArgs == 0)
	{
		Entity * const entity = EntityManager::Instance()->CreateEntity();
		lua_pushlightuserdata(luaState, entity);
	}
	else
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CreateEntity");
		lua_error(luaState);
	}

	return 1;
}

int ScriptingManager::lua_AddComponentAnimation(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddComponentAnimation");
		lua_error(luaState);
	}

	Entity * const entity = static_cast<Entity *>(lua_touserdata(luaState, 1));

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentAnimation");
		lua_error(luaState);
	}

	std::string function = lua_tostring(luaState, 2);

	EntityManager::Instance()->AddComponentToEntity(entity, new ComponentAnimation(function));

	return 0;
}

int ScriptingManager::lua_CreateAIStateMachine(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CreateAIStateMachine");
		lua_error(luaState);
	}

	std::string startFunction = lua_tostring(luaState, 1);

	AIStateMachine * const stateMachine = new AIStateMachine(startFunction);

	lua_pushlightuserdata(luaState, stateMachine);

	return 1;
}

int ScriptingManager::lua_CreatePathFinder(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 4)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CreatePathFinder");
		lua_error(luaState);
	}

	std::string target = lua_tostring(luaState, 1);
	std::string mapFile = lua_tostring(luaState, 2);

	float x = lua_tonumber(luaState, 3);
	float y = lua_tonumber(luaState, 4);

	glm::vec2 topLeftCoord(x, y);

	PathFinding * const pathFinder = new PathFinding(target, mapFile, topLeftCoord);

	lua_pushlightuserdata(luaState, pathFinder);

	return 1;
}

int ScriptingManager::lua_CreatePathVector(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 0)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CreatePathVector");
		lua_error(luaState);
	}

	std::vector<PathNode *> * const pathNodes = new std::vector<PathNode *>();

	lua_pushlightuserdata(luaState, pathNodes);

	return 1;
}

int ScriptingManager::lua_AddToPathVector(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 3)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddToPathVector");
		lua_error(luaState);
	}

	std::vector<PathNode *> * const pathNodes = static_cast<std::vector<PathNode *> *>(lua_touserdata(luaState, 1));

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	const glm::vec3 position(x, y, z);

	double radius = lua_tonumber(luaState, 3);

	PathNode * const pathNode = new PathNode();
	pathNode->position = position;
	pathNode->radius = radius;

	pathNodes->push_back(pathNode);

	return 0;
}

int ScriptingManager::lua_CreatePathFollower(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CreatePathFollower");
		lua_error(luaState);
	}

	std::vector<PathNode *> * pathNodes = static_cast<std::vector<PathNode *> *>(lua_touserdata(luaState, 1));

	PathFollowing * const pathFollower = new PathFollowing(pathNodes);

	lua_pushlightuserdata(luaState, pathFollower);

	return 1;
}

int ScriptingManager::lua_AddComponentArtificalIntelligence(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 5)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddComponentArtificialIntelligence");
		lua_error(luaState);
	}

	Entity * const entity = static_cast<Entity *>(lua_touserdata(luaState, 1));

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentArtificialIntelligence");
		lua_error(luaState);
	}

	PathFollowing * pathFollower = nullptr;

	if (!lua_isnil(luaState, 2))
	{
		pathFollower = static_cast<PathFollowing *>(lua_touserdata(luaState, 2));

		if (!pathFollower)
		{
			lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentArtificialIntelligence");
			lua_error(luaState);
		}
	}

	PathFinding * pathFinder = nullptr;

	if (!lua_isnil(luaState, 3))
	{
		pathFinder = static_cast<PathFinding *>(lua_touserdata(luaState, 3));

		if (!pathFinder)
		{
			lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentArtificialIntelligence");
			lua_error(luaState);
		}
	}

	AIStateMachine * stateMachine = nullptr;

	if (!lua_isnil(luaState, 4))
	{
		stateMachine = static_cast<AIStateMachine *>(lua_touserdata(luaState, 4));

		if (!stateMachine)
		{
			lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentArtificialIntelligence");
			lua_error(luaState);
		}
	}

	std::string target = lua_tostring(luaState, 5);

	EntityManager::Instance()->AddComponentToEntity(entity, new ComponentArtificalIntelligence(pathFollower, pathFinder, stateMachine, target));

	return 0;
}

int ScriptingManager::lua_AddComponentAudio(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 3)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddComponentAudio");
		lua_error(luaState);
	}

	Entity * const entity = static_cast<Entity *>(lua_touserdata(luaState, 1));

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentAudio");
		lua_error(luaState);
	}

	std::string playback = lua_tostring(luaState, 2);
	std::string audio = lua_tostring(luaState, 3);

	AudioPlayback playbackState;

	if (playback == "Play")
	{
		playbackState = AudioPlayback::PLAY;
	}
	else if (playback == "Pause")
	{
		playbackState = AudioPlayback::PAUSE;
	}
	else if (playback == "Stop")
	{
		playbackState = AudioPlayback::STOP;
	}
	else
	{
		lua_pushstring(luaState, "Wrong Playback State Passed in: AddComponentAudio");
		lua_error(luaState);
	}

	EntityManager::Instance()->AddComponentToEntity(entity, new ComponentAudio(playbackState, audio));

	return 0;
}

int ScriptingManager::lua_AddComponentDirection(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 3)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddComponentDirection");
		lua_error(luaState);
	}

	Entity * const entity = static_cast<Entity *>(lua_touserdata(luaState, 1));

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentDireciton");
		lua_error(luaState);
	}

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	glm::vec3 direction(x, y, z);
	float angle = lua_tonumber(luaState, 3);

	lua_pop(luaState, 4);

	EntityManager::Instance()->AddComponentToEntity(entity, new ComponentDirection(direction, angle));

	return 0;
}

int ScriptingManager::lua_CreateInputFunction(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CreateInputFunction");
		lua_error(luaState);
	}

	std::string functionName = lua_tostring(luaState, 1);

	InputFunction * const inputFunction = new InputFunction(functionName);

	lua_pushlightuserdata(luaState, inputFunction);

	return 1;
}

int ScriptingManager::lua_AddInputToInputFunction(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddInputToInputFunction");
		lua_error(luaState);
	}

	InputFunction * const inputFunction = static_cast<InputFunction *>(lua_touserdata(luaState, 1));

	if (!inputFunction)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddInputToInputFunction");
		lua_error(luaState);
	}

	std::string inputName = lua_tostring(luaState, 2);

	const GameInput input = InputConverter::StringToEnum(inputName);

	inputFunction->AddInput(input);

	return 0;
}

int ScriptingManager::lua_CreateInputVector(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 0)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CreateInputVector");
		lua_error(luaState);
	}

	std::vector<InputFunction *> * const gameInputs = new std::vector<InputFunction *>();

	lua_pushlightuserdata(luaState, gameInputs);

	return 1;
}

int ScriptingManager::lua_AddToInputVector(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddToInputVector");
		lua_error(luaState);
	}

	std::vector<InputFunction *> * const gameInputs = (std::vector<InputFunction *> *)lua_touserdata(luaState, 1);

	if (!gameInputs)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddToInputVector");
		lua_error(luaState);
	}

	InputFunction * const inputFunction = (InputFunction *)lua_touserdata(luaState, 2);

	if (!inputFunction)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddToInputVecotor");
		lua_error(luaState);
	}

	gameInputs->push_back(inputFunction);

	return 0;
}

int ScriptingManager::lua_AddComponentInput(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddComponentInput");
		lua_error(luaState);
	}

	Entity * const entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentInput");
		lua_error(luaState);
	}

	std::vector<InputFunction *> * gameInputs = (std::vector<InputFunction *> *)lua_touserdata(luaState, 2);

	if (!gameInputs)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentInput");
		lua_error(luaState);
	}

	EntityManager::Instance()->AddComponentToEntity(entity, new ComponentInput(gameInputs));

	return 0;
}

int ScriptingManager::lua_AddComponentModel(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number of Args: AddComponentModel");
		lua_error(luaState);
	}

	Entity * const entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentModel");
		lua_error(luaState);
	}

	std::string model = lua_tostring(luaState, 2);

	EntityManager::Instance()->AddComponentToEntity(entity, new ComponentModel(model));

	return 0;
}

int ScriptingManager::lua_AddComponentNormalTexture(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddComponentNormalTexture");
		lua_error(luaState);
	}

	Entity * const entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentNormalTexture");
		lua_error(luaState);
	}

	std::string texture = lua_tostring(luaState, 2);

	EntityManager::Instance()->AddComponentToEntity(entity, new ComponentNormalTexture(texture));

	return 0;
}

int ScriptingManager::lua_CreateCollisionSphere(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CreateCollisionSphere");
		lua_error(luaState);
	}

	float radius = lua_tonumber(luaState, 1);

	CollisionSphere * const sphere = new CollisionSphere(radius);

	lua_pushlightuserdata(luaState, sphere);

	return 1;
}

int ScriptingManager::lua_CreateCollisionCuboid(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CreateCollisionCuboid");
		lua_error(luaState);
	}

	lua_getfield(luaState, 1, "x");
	lua_getfield(luaState, 1, "y");
	lua_getfield(luaState, 1, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	glm::vec3 size(x, y, z);

	CollisionCuboid * const cuboid = new CollisionCuboid(size);

	lua_pushlightuserdata(luaState, cuboid);

	return 1;
}

int ScriptingManager::lua_CreateCollisionFunctionMap(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 0)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: CreateCollisionFunctionMap");
		lua_error(luaState);
	}

	std::map<EntityType, std::string> * const collisionFunctions = new std::map<EntityType, std::string>();

	lua_pushlightuserdata(luaState, collisionFunctions);

	return 1;
}

int ScriptingManager::lua_AddToCollisionFunctionMap(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 3)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddToCollisionFunctionMap");
		lua_error(luaState);
	}

	std::map<EntityType, std::string> * const collisionFunctions = (std::map<EntityType, std::string> *)lua_touserdata(luaState, 1);

	if (!collisionFunctions)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddToCollisionFunctionMap");
		lua_error(luaState);
	}

	std::string type = lua_tostring(luaState, 2);
	std::string function = lua_tostring(luaState, 3);

	EntityType entityType = EntityType::NONE;

	if (type == "Collectable")
	{
		entityType = EntityType::COLLECTABLE;
	}
	else if (type == "Player")
	{
		entityType = EntityType::PLAYER;
	}
	else if (type == "Wall")
	{
		entityType = EntityType::WALL;
	}
	else if (type != "None")
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddToCollisionFunctionMap");
		lua_error(luaState);
	}

	collisionFunctions->insert(std::pair<EntityType, std::string>(entityType, function));

	return 0;
}

int ScriptingManager::lua_AddComponentPhysics(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs < 5 || numberOfArgs > 7)
	{
		lua_pushstring(luaState, "Wrong Number of Args: AddComponentPhysics");
		lua_error(luaState);
	}

	Entity * const entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentPhysics");
		lua_error(luaState);
	}

	CollisionShape * shape = (CollisionShape *)lua_touserdata(luaState, 2);

	if (!shape)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentPhysics");
		lua_error(luaState);
	}

	float mass = lua_tonumber(luaState, 3);

	std::string type = lua_tostring(luaState, 4);

	EntityType entityType = EntityType::NONE;

	if (type == "Collectable")
	{
		entityType = EntityType::COLLECTABLE;
	}
	else if (type == "Player")
	{
		entityType = EntityType::PLAYER;
	}
	else if (type == "Wall")
	{
		entityType = EntityType::WALL;
	}
	else if (type != "None")
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddToCollisionFunctionMap");
		lua_error(luaState);
	}

	lua_getfield(luaState, 5, "x");
	lua_getfield(luaState, 5, "y");
	lua_getfield(luaState, 5, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	lua_pop(luaState, 3);

	glm::vec3 angularLimits(x, y, z);

	if (numberOfArgs == 5)
	{
		EntityManager::Instance()->AddComponentToEntity(entity, new ComponentPhysics(shape, mass, entityType, entity, angularLimits));
	}
	else
	{
		bool collisionResponse = lua_toboolean(luaState, 6);

		if (numberOfArgs == 6)
		{
			EntityManager::Instance()->AddComponentToEntity(entity, new ComponentPhysics(shape, mass, entityType, entity, angularLimits, collisionResponse));
		}
		else
		{
			std::map<EntityType, std::string> * const collisionFunctions = (std::map<EntityType, std::string> *)lua_touserdata(luaState, 7);

			if (!collisionFunctions)
			{
				lua_pushstring(luaState, "Wrong Parameters Passed in: AddToCollisionFunctionMap");
				lua_error(luaState);
			}

			EntityManager::Instance()->AddComponentToEntity(entity, new ComponentPhysics(shape, mass, entityType, entity, angularLimits, collisionResponse, collisionFunctions));
		}
	}

	return 0;
}

int ScriptingManager::lua_AddComponentPosition(lua_State * luaState)
{
	int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddComponentPosition");
		lua_error(luaState);
	}

	Entity * entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentPosition");
		lua_error(luaState);
	}

	lua_getfield(luaState, 2, "x");
	lua_getfield(luaState, 2, "y");
	lua_getfield(luaState, 2, "z");

	double x = lua_tonumber(luaState, -3);
	double y = lua_tonumber(luaState, -2);
	double z = lua_tonumber(luaState, -1);

	glm::vec3 position(x, y, z);

	EntityManager::Instance()->AddComponentToEntity(entity, new ComponentPosition(position));

	return 0;
}

int ScriptingManager::lua_AddComponentRiggedAnimation(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 3)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddComponentRiggedAnimation");
		lua_error(luaState);
	}

	Entity * const entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentRiggedAnimation");
		lua_error(luaState);
	}

	std::string animation = lua_tostring(luaState, 2);
	std::string state = lua_tostring(luaState, 3);

	RiggedAnimationState animationState = RiggedAnimationState::NONE;

	if (state == "Play")
	{
		animationState = RiggedAnimationState::PLAY;
	}
	else if (state == "Pause")
	{
		animationState = RiggedAnimationState::PAUSE;
	}
	else if (state == "Stop")
	{
		animationState = RiggedAnimationState::STOP;
	}
	else
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentRiggedAnimmation");
		lua_error(luaState);
	}

	EntityManager::Instance()->AddComponentToEntity(entity, new ComponentRiggedAnimation(animation, animationState));

	return 0;
}

int ScriptingManager::lua_AddComponentShader(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddComponentShader");
		lua_error(luaState);
	}

	Entity * const entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentShader");
		lua_error(luaState);
	}

	std::string shader = lua_tostring(luaState, 2);

	EntityManager::Instance()->AddComponentToEntity(entity, new ComponentShader(shader));

	return 0;
}

int ScriptingManager::lua_AddComponentShadowShader(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 3)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddComponentShadowShader");
		lua_error(luaState);
	}

	Entity * const entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentShadowShader");
		lua_error(luaState);
	}

	std::string directionShader = lua_tostring(luaState, 2);
	std::string pointShader = lua_tostring(luaState, 3);

	EntityManager::Instance()->AddComponentToEntity(entity, new ComponentShadowShader(directionShader, pointShader));

	return 0;
}

int ScriptingManager::lua_AddComponentState(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddComponentState");
		lua_error(luaState);
	}

	Entity * const entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passeed in: AddComponentState");
		lua_error(luaState);
	}

	EntityManager::Instance()->AddComponentToEntity(entity, new ComponentState());

	return 0;
}

int ScriptingManager::lua_AddComponentTexture(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 2)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: AddComponentTexture");
		lua_error(luaState);
	}

	Entity * const entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: AddComponentTexture");
		lua_error(luaState);
	}

	std::string texture = lua_tostring(luaState, 2);

	EntityManager::Instance()->AddComponentToEntity(entity, new ComponentTexture(texture));

	return 0;
}

int ScriptingManager::lua_FinishEntity(lua_State * const luaState)
{
	const int numberOfArgs = lua_gettop(luaState);

	if (numberOfArgs != 1)
	{
		lua_pushstring(luaState, "Wrong Number Of Args: FinishEntity");
		lua_error(luaState);
	}

	Entity * const entity = (Entity *)lua_touserdata(luaState, 1);

	if (!entity)
	{
		lua_pushstring(luaState, "Wrong Parameters Passed in: FinishEntity");
		lua_error(luaState);
	}

	EntityManager::Instance()->FinishEntity(entity);

	return 0;
}

void ScriptingManager::RunScriptFromCollision(const std::string & function, Entity * const entity) const
{
	lua_getglobal(luaVM, function.c_str());
	lua_pushlightuserdata(luaVM, entity);

	if (lua_pcall(luaVM, 1, 0, 0) != 0)
	{
		std::string message = lua_tostring(luaVM, -1);
		message = "Error running lua script: " + message;
		LoggingManager::LogMessage(MESSAGE_TYPE::LOG, message);
	}
}

void ScriptingManager::RunScriptFromInput(const std::string & function, Entity * const entity, const float inputValue, const float deltaTime) const
{
	lua_getglobal(luaVM, function.c_str());
	lua_pushlightuserdata(luaVM, entity);
	lua_pushnumber(luaVM, inputValue);
	lua_pushnumber(luaVM, deltaTime);

	if (lua_pcall(luaVM, 3, 0, 0) != 0)
	{
		std::string message = lua_tostring(luaVM, -1);
		message = "Error running lua script: " + message;
		LoggingManager::LogMessage(MESSAGE_TYPE::LOG, message);
	}
}

void ScriptingManager::RunScriptForStateAI(const std::string & function, Entity * const entity1, Entity * const entity2, AIStateMachine * const stateMachine, const float deltaTime) const
{
	lua_getglobal(luaVM, function.c_str());
	lua_pushlightuserdata(luaVM, entity1);
	lua_pushlightuserdata(luaVM, entity2);
	lua_pushlightuserdata(luaVM, stateMachine);
	lua_pushnumber(luaVM, deltaTime);

	if (lua_pcall(luaVM, 4, 0, 0) != 0)
	{
		std::string message = lua_tostring(luaVM, -1);
		message = "Error running lua script: " + message;
		LoggingManager::LogMessage(MESSAGE_TYPE::LOG, message);
	}
}

void ScriptingManager::RunScriptFromInput(const std::string & function)
{
	lua_getglobal(luaVM, function.c_str());

	if (lua_pcall(luaVM, 0, 0, 0) != 0)
	{
		std::string message = lua_tostring(luaVM, -1);
		message = "Error running lua script: " + message;
		LoggingManager::LogMessage(MESSAGE_TYPE::LOG, message);
	}
}

void ScriptingManager::RunScriptFromAnimation(const std::string & function, Entity * const entity)
{
	lua_getglobal(luaVM, function.c_str());
	lua_pushlightuserdata(luaVM, entity);
	lua_pushnumber(luaVM, (1.0 / 60.0));

	if (lua_pcall(luaVM, 2, 0, 0) != 0)
	{
		std::string message = lua_tostring(luaVM, -1);
		message = "Error running lua script: " + message;
		LoggingManager::LogMessage(MESSAGE_TYPE::LOG, message);
	}
}

ScriptingManager * ScriptingManager::instance = nullptr;
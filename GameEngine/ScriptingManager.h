#pragma once

extern "C"
{
#include "Lua/lua.h"
}

#include "Entity.h"
#include "AIStateMachine.h"
#include <string>

class ScriptingManager
{
private:
	static ScriptingManager * instance;

	static int lua_GetComponentDirection(lua_State * luaState);
	static int lua_GetComponentPosition(lua_State * luaState);
	static int lua_GetComponentPhysics(lua_State * luaState);
	static int lua_GetComponentAudio(lua_State * luaState);
	static int lua_GetComponentState(lua_State * luaState);
	static int lua_GetPosition(lua_State * luaState);
	static int lua_SetPosition(lua_State * luaState);
	static int lua_GetVelocity(lua_State * luaState);
	static int lua_SetVelocity(lua_State * luaState);
	static int lua_GetImpulse(lua_State * luaState);
	static int lua_SetImpulse(lua_State * luaState);
	static int lua_GetRotation(lua_State * luaState);
	static int lua_SetRotation(lua_State * luaState);
	static int lua_GetDirection(lua_State * luaState);
	static int lua_SetDirection(lua_State * luaState);
	static int lua_GetTouchingGround(lua_State * luaState);
	static int lua_CreateTranslationMatrix(lua_State * luaState);
	static int lua_CreateScaleMatrix(lua_State * luaState);
	static int lua_CreateRotationMatrix(lua_State * luaState);
	static int lua_MultiplyMatrices(lua_State * luaState);
	static int lua_MultiplyMatrixVector(lua_State * luaState);
	static int lua_ChangePlayback(lua_State * luaState);
	static int lua_PlayAudio(lua_State * luaState);
	static int lua_PlayAudioAtLocation(lua_State * luaState);
	static int lua_PlayAudioAtEntityLocation(lua_State * luaState);
	static int lua_DeleteEntity(lua_State * luaState);
	static int lua_GetAIValue(lua_State * luaState);
	static int lua_SetAIValue(lua_State * luaState);
	static int lua_GetValue(lua_State * luaState);
	static int lua_SetValue(lua_State * luaState);
	static int lua_CanSeePlayer(lua_State * luaState);
	static int lua_MoveOffPath(lua_State * luaState);
	static int lua_OnPath(lua_State * luaState);
	static int lua_FindAIPath(lua_State * luaState);
	static int lua_FindPath(lua_State * luaState);
	static int lua_GetNearestPath(lua_State * luaState);
	static int lua_NewGameScene(lua_State * luaState);
	static int lua_NewMenuScene(lua_State * luaState);
	static int lua_SwapToMenuScene(lua_State * luaState);
	static int lua_SwapToGameScene(lua_State * luaState);
	static int lua_CloseScene(lua_State * luaState);
	static int lua_CloseWindow(lua_State * luaState);

	static int lua_CreateEntity(lua_State * luaState);
	static int lua_GetEntity(lua_State * luaState);
	static int lua_AddComponentAnimation(lua_State * luaState);
	static int lua_CreateAIStateMachine(lua_State * luaState);
	static int lua_CreatePathFinder(lua_State * luaState);
	static int lua_CreatePathVector(lua_State * luaState);
	static int lua_AddToPathVector(lua_State * luaState);
	static int lua_CreatePathFollower(lua_State * luaState);
	static int lua_AddComponentArtificalIntelligence(lua_State * luaState);
	static int lua_AddComponentAudio(lua_State * luaState);
	static int lua_AddComponentDirection(lua_State * luaState);
	static int lua_CreateInputFunction(lua_State * luaState);
	static int lua_AddInputToInputFunction(lua_State * luaState);
	static int lua_CreateInputVector(lua_State * luaState);
	static int lua_AddToInputVector(lua_State * luaState);
	static int lua_AddComponentInput(lua_State * luaState);
	static int lua_AddComponentModel(lua_State * luaState);
	static int lua_AddComponentNormalTexture(lua_State * luaState);
	static int lua_CreateCollisionSphere(lua_State * luaState);
	static int lua_CreateCollisionCuboid(lua_State * luaState);
	static int lua_CreateCollisionFunctionMap(lua_State * luaState);
	static int lua_AddToCollisionFunctionMap(lua_State * luaState);
	static int lua_AddComponentPhysics(lua_State * luaState);
	static int lua_AddComponentPosition(lua_State * luaState);
	static int lua_AddComponentRiggedAnimation(lua_State * luaState);
	static int lua_AddComponentShader(lua_State * luaState);
	static int lua_AddComponentShadowShader(lua_State * luaState);
	static int lua_AddComponentState(lua_State * luaState);
	static int lua_AddComponentTexture(lua_State * luaState);
	static int lua_FinishEntity(lua_State * luaState);

	static int lua_DisplayText(lua_State * luaState);

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

	ScriptingManager& operator=(const ScriptingManager&) = delete;
	ScriptingManager(ScriptingManager&) = delete;

	void LoadLuaFromFile(const std::string & fileName) const;

	void RunScriptFromCollision(const std::string & function, Entity * entity) const;

	void RunScriptFromInput(const std::string & function, Entity * entity, float inputValue, float deltaTime) const;

	void RunScriptForStateAI(const std::string & function, Entity * entity1, Entity * entity2, AIStateMachine * stateMachine, float deltaTime) const;

	void RunScriptFromScene(const std::string & function);

	void RunScriptFromAnimation(const std::string & function, Entity * entity);

	~ScriptingManager();
};


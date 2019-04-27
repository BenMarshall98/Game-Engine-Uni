#pragma once
#include "PathFollowing.h"
#include "PathFinding.h"
#include "AIStateMachine.h"
#include "Entity.h"
#include "ComponentPhysics.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"

class ComponentArtificalIntelligence : public iComponent
{
private:
	PathFollowing * pathFollower;
	PathFinding * pathFinding;
	AIStateMachine * aiStateMachine;
	Entity * target;

public:
	explicit ComponentArtificalIntelligence(PathFollowing * const pPathFollower, PathFinding * const pPathFinding, AIStateMachine * const pAIStateMachine, const std::string & pTarget) :
		iComponent(ComponentType::COMPONENT_ARTIFICAL_INTELLIGENCE), pathFollower(pPathFollower), pathFinding(pPathFinding),
		aiStateMachine(pAIStateMachine), target(EntityManager::Instance()->GetEntityByName(pTarget))
	{
		if (aiStateMachine)
		{
			aiStateMachine->SetPathFinder(pathFinding);
			aiStateMachine->SetPathFollower(pathFollower);
		}
	}
	~ComponentArtificalIntelligence();

	ComponentArtificalIntelligence& operator=(const ComponentArtificalIntelligence&) = delete;
	ComponentArtificalIntelligence(ComponentArtificalIntelligence&) = delete;

	void RenderSwap() override;

	//Calls the AI pointer, according to preferance AIStateMachine > PathFinding > PathFollowing
	inline void ResolveAI(Entity * const thisEntity, float timePeriod)
	{
		if (aiStateMachine)
		{
			aiStateMachine->ResolveStates(thisEntity, target, timePeriod);
		}
		else
		{
			iComponent * const componentPosition = EntityManager::Instance()->GetComponentOfEntity(thisEntity, ComponentType::COMPONENT_POSITION);
			iComponent * const componentDirection = EntityManager::Instance()->GetComponentOfEntity(thisEntity, ComponentType::COMPONENT_DIRECTION);
			iComponent * const componentPhysics = EntityManager::Instance()->GetComponentOfEntity(thisEntity, ComponentType::COMPONENT_PHYSICS);

			const glm::vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetRenderPosition();
			const glm::quat direction = dynamic_cast<ComponentDirection *>(componentDirection)->GetRenderDirection();
			ComponentPhysics * const physics = dynamic_cast<ComponentPhysics *>(componentPhysics);

			if (pathFinding)
			{
				pathFinding->CalculatePath(position, direction, physics);
			}
			else if (pathFollower)
			{
				pathFollower->CalculatePath(position, direction, physics);
			}
		}
		

		
	}
};


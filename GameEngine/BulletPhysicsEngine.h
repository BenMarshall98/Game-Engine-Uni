#pragma once

#include "..\BulletPhysics\btBulletDynamicsCommon.h"
#include "CollisionShape.h"
#include "glm/glm.hpp"

using namespace glm;

class BulletPhysicsEngine
{
private:
	btDiscreteDynamicsWorld * dynamicsWorld;
public:
	BulletPhysicsEngine();
	~BulletPhysicsEngine();
	
	btRigidBody* AddRigidBody(float mass, vec3 position, quat direction, CollisionShape * shape);

	inline void Update(float pDeltaTime)
	{
		dynamicsWorld->stepSimulation(pDeltaTime);
	}

	vec3 GetPositionOfRigidBody(void * pRigidBody);
	quat GetDirectionOfRigidBody(void * pRigidBody);
	void ApplyForce(void * pRigidBody, vec3 force);
	void ApplyImpulse(void * pRigidBody, vec3 impulse);
};


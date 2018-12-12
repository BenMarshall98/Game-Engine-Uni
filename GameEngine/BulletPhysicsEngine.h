#pragma once

#include "..\BulletPhysics\btBulletDynamicsCommon.h"
#include "CollisionShape.h"
#include "glm/glm.hpp"
#include "Entity.h"
#include "ComponentPhysics.h"

using namespace glm;

class BulletPhysicsEngine
{
private:

	static btDiscreteDynamicsWorld * dynamicsWorld;

	static bool collisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2);
	static bool TouchingGround(const void * pRigidBody1, const void * pRigidBody2);
public:
	BulletPhysicsEngine();
	~BulletPhysicsEngine();

	btRigidBody* AddRigidBody(float mass, vec3 position, quat direction, CollisionShape * shape, Entity * entity, bool collisionResponse);

	inline void Update(float pDeltaTime)
	{
		dynamicsWorld->stepSimulation(pDeltaTime);
	}

	vec3 GetPositionOfRigidBody(void * pRigidBody);
	quat GetDirectionOfRigidBody(void * pRigidBody);
	void ApplyVelocity(void * pRigidBody, vec3 velocity);
	void ApplyImpulse(void * pRigidBody, vec3 impulse);
};
#pragma once

#include "..\BulletPhysics\btBulletDynamicsCommon.h"
#include "CollisionShape.h"
#include "glm/glm.hpp"
#include "Entity.h"
#include "ComponentPhysics.h"
#include "PhysicsEngine.h"

using namespace glm;

class BulletPhysicsEngine : public PhysicsEngine
{
private:

	static btDiscreteDynamicsWorld * dynamicsWorld;

	static bool collisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2);
	static bool TouchingGround(const void * pRigidBody1, const void * pRigidBody2);
public:
	BulletPhysicsEngine();
	~BulletPhysicsEngine();

	RigidBody * AddRigidBody(float mass, vec3 & position, quat & direction, CollisionShape * shape, Entity * entity, bool collisionResponse, vec3 & angularLimit) override;

	void Update(float pDeltaTime) override;

	void RemoveRigidBody(RigidBody * pRigidBody) override;

	bool ClearBetweenPoints(vec3 position1, vec3 position2) override;
};
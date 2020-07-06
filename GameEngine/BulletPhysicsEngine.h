#pragma once

#include "BulletPhysics\btBulletDynamicsCommon.h"
#include "CollisionShape.h"
#include "glm/glm.hpp"
#include "Entity.h"
#include "ComponentPhysics.h"
#include "PhysicsEngine.h"
#include <vector>

class BulletPhysicsEngine : public PhysicsEngine
{
private:

	static btDiscreteDynamicsWorld * dynamicsWorld;
	btSequentialImpulseConstraintSolver* solver;
	btCollisionDispatcher* dispatcher;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btBroadphaseInterface* broadphase;

	std::vector<btCollisionShape *> collisionShapes;

	static bool collisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper * const obj1, const int id1, const int index1, const btCollisionObjectWrapper * const obj2, const int id2, const int index2);
	static bool TouchingGround(const btCollisionObject * pRigidBody1, const btCollisionObject * pRigidBody2);
public:
	BulletPhysicsEngine();
	~BulletPhysicsEngine();

	BulletPhysicsEngine& operator=(const BulletPhysicsEngine&) = delete;
	BulletPhysicsEngine(BulletPhysicsEngine&) = delete;

	RigidBody * AddRigidBody(const float mass, glm::vec3 & position, glm::quat & direction, CollisionShape * const shape, Entity * const entity, const bool collisionResponse, const glm::vec3 & angularLimit) override;

	void Update(float pDeltaTime) override;

	void RemoveRigidBody(RigidBody * pRigidBody) override;

	bool ClearBetweenPoints(const glm::vec3 & position1, const glm::vec3 & position2) override;
};
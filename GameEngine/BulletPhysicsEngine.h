#pragma once

#include "..\BulletPhysics\btBulletDynamicsCommon.h"
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

	static bool collisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2);
	static bool TouchingGround(const void * pRigidBody1, const void * pRigidBody2);
public:
	BulletPhysicsEngine();
	~BulletPhysicsEngine();

	RigidBody * AddRigidBody(float mass, glm::vec3 & position, glm::quat & direction, CollisionShape * shape, Entity * entity, bool collisionResponse, glm::vec3 & angularLimit) override;

	void Update(float pDeltaTime) override;
	void Clear() override;

	void RemoveRigidBody(RigidBody * pRigidBody) override;

	bool ClearBetweenPoints(glm::vec3 position1, glm::vec3 position2) override;
};
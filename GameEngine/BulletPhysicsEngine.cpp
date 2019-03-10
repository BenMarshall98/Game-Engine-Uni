
#include "BulletPhysicsEngine.h"
#include "CollisionCuboid.h"
#include "CollisionSphere.h"
#include "EntityManager.h"
#include "BulletRigidBody.h"
#include <iostream>

#include "glm/gtc/quaternion.hpp"

BulletPhysicsEngine::BulletPhysicsEngine()
{
	//Build the broadphase
	broadphase = new btDbvtBroadphase();

	//Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//The actual physics solver
	solver = new btSequentialImpulseConstraintSolver;

	//The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

	gContactAddedCallback = collisionCallback;
}

BulletPhysicsEngine::~BulletPhysicsEngine()
{
	CProfileManager::CleanupMemory();

	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject * const obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody * const body = btRigidBody::upcast(obj);

		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}

		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	for (int i = 0; i < collisionShapes.size(); i++)
	{
		delete collisionShapes[i];
		collisionShapes[i] = nullptr;
	}

	collisionShapes.clear();

	delete dynamicsWorld;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfiguration;
}

RigidBody* BulletPhysicsEngine::AddRigidBody(const float mass, glm::vec3 & position, glm::quat & direction, CollisionShape * const shape, Entity * const entity, const bool collisionResponse, const glm::vec3 & angularLimit)
{
	btCollisionShape* collisionShape;

	switch (shape->GetShape())
	{
		case Shape::CUBOID:
			{
				CollisionCuboid * const cuboid = dynamic_cast<CollisionCuboid *>(shape);
				glm::vec3 size = cuboid->GetSize();
				collisionShape = new btBoxShape(btVector3(size.x, size.y, size.z));
			}
			break;
		case Shape::SPHERE:
			{
				CollisionSphere * const sphere = dynamic_cast<CollisionSphere *>(shape);
				float radius = sphere->GetRadius();
				collisionShape = new btSphereShape(radius);
			}
			break;
		default:
			collisionShape = nullptr;
	}


	collisionShapes.push_back(collisionShape);

	btDefaultMotionState * const motionState = new btDefaultMotionState(btTransform(
		btQuaternion(direction.x, direction.y, direction.z, direction.w),
		btVector3(position.x, position.y, position.z)
	));

	btVector3 inertia;
	collisionShape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, inertia);

	btRigidBody * rigidBody = new btRigidBody(rigidBodyCI);

	rigidBody->setUserPointer(entity);

	if (mass != 0)
	{
		rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	}
	else
	{
		rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	}

	if (!collisionResponse)
	{
		rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}

	const btVector3 angLimit = btVector3(angularLimit.x, angularLimit.y, angularLimit.z);

	rigidBody->setAngularFactor(angLimit);

	dynamicsWorld->addRigidBody(rigidBody);

	RigidBody * const rBody = new BulletRigidBody(rigidBody);

	return rBody;
}

void BulletPhysicsEngine::Update(const float pDeltaTime)
{
	dynamicsWorld->stepSimulation(pDeltaTime);
}

bool BulletPhysicsEngine::collisionCallback(const btManifoldPoint& cp, const btCollisionObjectWrapper * const obj1, const int id1, const int index1, const btCollisionObjectWrapper * const obj2, const int id2, const int index2)
{
	Entity * const entity1 = (Entity *)obj1->getCollisionObject()->getUserPointer();
	Entity * const entity2 = (Entity *)obj2->getCollisionObject()->getUserPointer();

	iComponent * const physicsComponent1 = EntityManager::Instance()->GetComponentOfEntity(entity1, ComponentType::COMPONENT_PHYSICS);
	iComponent * const physicsComponent2 = EntityManager::Instance()->GetComponentOfEntity(entity2, ComponentType::COMPONENT_PHYSICS);

	ComponentPhysics * const componentPhysics1 = (ComponentPhysics *)physicsComponent1;
	ComponentPhysics * const componentPhysics2 = (ComponentPhysics *)physicsComponent2;

	componentPhysics1->AddCollision(entity2, componentPhysics2->GetUpdateEntityType());
	componentPhysics2->AddCollision(entity1, componentPhysics1->GetUpdateEntityType());

	if (componentPhysics1->GetUpdateMass() != 0 && componentPhysics2->GetUpdateEntityType() == EntityType::WALL)
	{
		const bool touchingGround = TouchingGround(obj1->getCollisionObject(), obj2->getCollisionObject());
		componentPhysics1->SetUpdateTouchingGround(touchingGround);
	}
	if (componentPhysics2->GetUpdateMass() != 0 && componentPhysics1->GetUpdateEntityType() == EntityType::WALL)
	{
		const bool touchingGround = TouchingGround(obj2->getCollisionObject(), obj1->getCollisionObject());
		componentPhysics2->SetUpdateTouchingGround(touchingGround);
	}
	return false;
}

bool BulletPhysicsEngine::TouchingGround(const void * const pRigidBody1, const void * const pRigidBody2)
{
	btRigidBody * const rigidBody = (btRigidBody *)pRigidBody1;

	btCollisionShape * const collisionShape = rigidBody->getCollisionShape();

	const btVector3 rayStart = rigidBody->getWorldTransform().getOrigin();
	btVector3 offset;
	btScalar radius;
	collisionShape->getBoundingSphere(offset, radius);

	const btVector3 rayEnd = btVector3(rayStart.x(), rayStart.y() - radius, rayStart.z());

	btCollisionWorld::AllHitsRayResultCallback res(rayStart, rayEnd);

	dynamicsWorld->rayTest(rayStart, rayEnd, res);

	if (res.hasHit())
	{
		btAlignedObjectArray<const btCollisionObject *> collisionObjects = res.m_collisionObjects;

		for (int i = 0; i < collisionObjects.size(); i++)
		{
			if (collisionObjects.at(i) == pRigidBody2)
			{
				return true;
			}
		}
	}

	return false;
}

void BulletPhysicsEngine::RemoveRigidBody(RigidBody * const pRigidBody)
{
	btRigidBody * const rigidBody = (btRigidBody *)pRigidBody->GetRigidBody();

	if (rigidBody && rigidBody->getMotionState())
	{
		delete rigidBody->getMotionState();
	}

	dynamicsWorld->removeRigidBody(rigidBody);
}

bool BulletPhysicsEngine::ClearBetweenPoints(const glm::vec3 position1, const glm::vec3 position2)
{
	const btVector3 rayStart = btVector3(position1.x, position1.y, position1.z);
	const btVector3 rayEnd = btVector3(position2.x, position2.y, position2.z);

	btCollisionWorld::AllHitsRayResultCallback res(rayStart, rayEnd);

	dynamicsWorld->rayTest(rayStart, rayEnd, res);

	if (res.hasHit())
	{
		btAlignedObjectArray<const btCollisionObject *> collisionObjects = res.m_collisionObjects;

		for (int i = 0; i < collisionObjects.size(); i++)
		{
			const btVector3 collisionPosition = collisionObjects.at(i)->getWorldTransform().getOrigin();
			if (collisionPosition.distance(rayStart) > 0.05 &&
				collisionPosition.distance(rayEnd) > 0.05)
			{
				return false;
			}
		}
	}

	return true;
}

btDiscreteDynamicsWorld * BulletPhysicsEngine::dynamicsWorld = nullptr;
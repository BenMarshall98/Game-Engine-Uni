
#include "BulletPhysicsEngine.h"
#include "CollisionCuboid.h"
#include "CollisionSphere.h"
#include "EntityManager.h"
#include <iostream>

#include "glm/gtc/quaternion.hpp"

BulletPhysicsEngine::BulletPhysicsEngine()
{
	//Build the broadphase
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	//Set up the collision configuration and dispatcher
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//The actual physics solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	//The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

	gContactAddedCallback = collisionCallback;
}

BulletPhysicsEngine::~BulletPhysicsEngine()
{
}

btRigidBody* BulletPhysicsEngine::AddRigidBody(float mass, vec3 & position, quat & direction, CollisionShape * shape, Entity * entity, bool collisionResponse, vec3 & angularLimit)
{
	btCollisionShape* collisionShape;

	switch (shape->GetShape())
	{
		case Shape::CUBOID:
			{
				CollisionCuboid * cuboid = dynamic_cast<CollisionCuboid *>(shape);
				vec3 size = cuboid->GetSize();
				collisionShape = new btBoxShape(btVector3(size.x, size.y, size.z));
			}
			break;
		case Shape::SPHERE:
			{
				CollisionSphere * sphere = dynamic_cast<CollisionSphere *>(shape);
				float radius = sphere->GetRadius();
				collisionShape = new btSphereShape(radius);
			}
			break;
		default:
			collisionShape = nullptr;
	}


	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(
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

	if (!collisionResponse)
	{
		rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}

	btVector3 angLimit = btVector3(angularLimit.x, angularLimit.y, angularLimit.z);

	rigidBody->setAngularFactor(angLimit);

	dynamicsWorld->addRigidBody(rigidBody);

	return rigidBody;
}

vec3 BulletPhysicsEngine::GetPositionOfRigidBody(void * pRigidBody)
{
	btRigidBody * rigidBody = (btRigidBody* )pRigidBody;

	rigidBody->activate();

	btTransform transform = rigidBody->getWorldTransform();
	btVector3 origin = transform.getOrigin();

	return vec3(origin.x(), origin.y(), origin.z());
}

quat BulletPhysicsEngine::GetDirectionOfRigidBody(void * pRigidBody)
{
	btRigidBody * rigidBody = (btRigidBody*)pRigidBody;

	btTransform transform = rigidBody->getWorldTransform();
	btQuaternion rotation = transform.getRotation();

	return quat(rotation.w(), rotation.x(), rotation.y(), rotation.z());
}

void BulletPhysicsEngine::ApplyVelocity(void * pRigidBody, vec3 & pVelocity) const
{
	btRigidBody * rigidBody = (btRigidBody*)pRigidBody;

	btVector3 currentVel = rigidBody->getLinearVelocity();
	vec3 currentVelocity(currentVel.x(), currentVel.y(), currentVel.z());
	vec3 desiredVelocity(pVelocity.x, currentVel.y(), pVelocity.z);

	vec3 velocity = mix(currentVelocity, desiredVelocity, 0.1);

	btVector3 vel(velocity.x, velocity.y, velocity.z);
	
	rigidBody->setLinearVelocity(vel);
}

void BulletPhysicsEngine::ApplyImpulse(void * pRigidBody, vec3 & pImpulse) const
{
	btRigidBody * rigidBody = (btRigidBody*)pRigidBody;

	btVector3 origin(0.0, 0.0, 0.0);
	btVector3 impulse(pImpulse.x, pImpulse.y, pImpulse.z);
	rigidBody->applyImpulse(impulse, origin);
}

void BulletPhysicsEngine::ApplyRotation(void * pRigidBody, vec3 & pRotation) const
{
	btRigidBody * rigidBody = (btRigidBody*)pRigidBody;

	btVector3 currentRot = rigidBody->getAngularVelocity();
	vec3 currentRotation(currentRot.x(), currentRot.y(), currentRot.z());
	vec3 desiredRotation(pRotation.x, pRotation.y, pRotation.z);

	vec3 rotation = mix(currentRotation, desiredRotation, 0.1);

	btVector3 rot(rotation.x, rotation.y, rotation.z);

	rigidBody->setAngularVelocity(rot);
}

bool BulletPhysicsEngine::collisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2)
{
	Entity * entity1 = (Entity *)obj1->getCollisionObject()->getUserPointer();
	Entity * entity2 = (Entity *)obj2->getCollisionObject()->getUserPointer();

	iComponent * physicsComponent1 = EntityManager::Instance()->GetComponentOfEntity(entity1, ComponentType::COMPONENT_PHYSICS);
	iComponent * physicsComponent2 = EntityManager::Instance()->GetComponentOfEntity(entity2, ComponentType::COMPONENT_PHYSICS);

	ComponentPhysics * componentPhysics1 = (ComponentPhysics *)physicsComponent1;
	ComponentPhysics * componentPhysics2 = (ComponentPhysics *)physicsComponent2;

	componentPhysics1->AddCollision(entity2, componentPhysics2->GetUpdateEntityType());
	componentPhysics2->AddCollision(entity1, componentPhysics1->GetUpdateEntityType());

	if (componentPhysics1->GetUpdateMass() != 0 && componentPhysics2->GetUpdateEntityType() == EntityType::WALL)
	{
		bool touchingGround = TouchingGround(obj1->getCollisionObject(), obj2->getCollisionObject());
		componentPhysics1->SetUpdateTouchingGround(touchingGround);
	}
	if (componentPhysics2->GetUpdateMass() != 0 && componentPhysics1->GetUpdateEntityType() == EntityType::WALL)
	{
		bool touchingGround = TouchingGround(obj2->getCollisionObject(), obj1->getCollisionObject());
		componentPhysics2->SetUpdateTouchingGround(touchingGround);
	}
	return false;
}

bool BulletPhysicsEngine::TouchingGround(const void * pRigidBody1, const void * pRigidBody2)
{
	btRigidBody * rigidBody = (btRigidBody *)pRigidBody1;

	btCollisionShape * collisionShape = rigidBody->getCollisionShape();

	btVector3 rayStart = rigidBody->getWorldTransform().getOrigin();
	btVector3 offset;
	btScalar radius;
	collisionShape->getBoundingSphere(offset, radius);

	btVector3 rayEnd = btVector3(rayStart.x(), rayStart.y() - radius, rayStart.z());

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

void BulletPhysicsEngine::RemoveRigidBody(void * pRigidBody)
{
	dynamicsWorld->removeRigidBody((btRigidBody *)pRigidBody);
}

btDiscreteDynamicsWorld * BulletPhysicsEngine::dynamicsWorld = nullptr;
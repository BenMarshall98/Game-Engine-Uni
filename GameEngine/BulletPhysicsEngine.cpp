
#include "BulletPhysicsEngine.h"
#include "CollisionCuboid.h"
#include "CollisionSphere.h"
#include "TestGameScene.h"
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

btRigidBody* BulletPhysicsEngine::AddRigidBody(float mass, vec3 position, quat direction, CollisionShape * shape, Entity * entity)
{
	btCollisionShape* collisionShape;

	switch (shape->GetShape())
	{
		case CUBOID:
			{
				CollisionCuboid * cuboid = dynamic_cast<CollisionCuboid *>(shape);
				vec3 size = cuboid->GetSize();
				collisionShape = new btBoxShape(btVector3(size.x, size.y, size.z));
			}
			break;
		case SPHERE:
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

	rigidBody->setAngularFactor(btVector3(0, 0, 1));
	rigidBody->setUserPointer(entity);

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

void BulletPhysicsEngine::ApplyVelocity(void * pRigidBody, vec3 pVelocity)
{
	btRigidBody * rigidBody = (btRigidBody*)pRigidBody;

	btVector3 currentVel = rigidBody->getLinearVelocity();
	vec3 currentVelocity(currentVel.x(), currentVel.y(), currentVel.z());
	vec3 desiredVelocity(pVelocity.x, currentVel.y(), pVelocity.z);

	vec3 velocity = mix(currentVelocity, desiredVelocity, 0.1);

	btVector3 vel(velocity.x, velocity.y, velocity.z);
	
	rigidBody->setLinearVelocity(vel);
}

void BulletPhysicsEngine::ApplyImpulse(void * pRigidBody, vec3 pImpulse)
{
	btRigidBody * rigidBody = (btRigidBody*)pRigidBody;

	btVector3 origin(0.0, 0.0, 0.0);
	btVector3 impulse(pImpulse.x, pImpulse.y, pImpulse.z);
	rigidBody->applyImpulse(impulse, origin);
}

bool BulletPhysicsEngine::collisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2)
{
	cout << "Collision!" << endl;
	
	Entity * entity1 = (Entity *)((btRigidBody *)obj1)->getUserPointer();
	Entity * entity2 = (Entity *)((btRigidBody *)obj2)->getUserPointer();

	iComponent * physicsComponent1 = TestGameScene::mEntityManager.GetComponentOfEntity(entity1, COMPONENT_PHYSICS);
	iComponent * physicsComponent2 = TestGameScene::mEntityManager.GetComponentOfEntity(entity2, COMPONENT_PHYSICS);

	ComponentPhysics * componentPhysics1 = (ComponentPhysics *)physicsComponent1;
	ComponentPhysics * componentPhysics2 = (ComponentPhysics *)physicsComponent2;

	componentPhysics1->AddCollision(entity2, componentPhysics2->GetEntityType());
	componentPhysics2->AddCollision(entity1, componentPhysics1->GetEntityType());

	if (componentPhysics1->GetMass() != 0)
	{
		bool touchingGround = TouchingGround((btRigidBody *)obj1, (btRigidBody *)obj2, componentPhysics2);
		componentPhysics1->SetTouchingGround(touchingGround);
	}

	if (componentPhysics2->GetMass() != 0)
	{
		bool touchingGround = TouchingGround((btRigidBody *)obj2, (btRigidBody *)obj1, componentPhysics1);
		componentPhysics2->SetTouchingGround(touchingGround);
	}

	return false;
}

bool BulletPhysicsEngine::TouchingGround(void * pRigidBody1, void * pRigidBody2, ComponentPhysics * componentPhysics2)
{
	btRigidBody * rigidBody = (btRigidBody*)pRigidBody1;
	btCollisionShape * collisionShape = rigidBody->getCollisionShape();

	btVector3 rayStart = rigidBody->getWorldTransform().getOrigin();
	btVector3 offset;
	btScalar radius;
	collisionShape->getBoundingSphere(offset, radius);

	btVector3 rayEnd = btVector3(rayStart.x(), rayStart.y() - radius, rayStart.z());

	btCollisionWorld::ClosestRayResultCallback res(rayStart, rayEnd);

	dynamicsWorld->rayTest(rayStart, rayEnd, res);

	if (res.hasHit())
	{
		const btCollisionObject * object = res.m_collisionObject;
		const btRigidBody * collideRigidBody = (btRigidBody *)object;

		if (collideRigidBody == pRigidBody2)
		{
			return componentPhysics2->GetEntityType() == WALL;
		}
	}

	return false;
}

btDiscreteDynamicsWorld * BulletPhysicsEngine::dynamicsWorld = nullptr;
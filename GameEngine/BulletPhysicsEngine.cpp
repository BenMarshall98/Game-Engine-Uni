
#include "BulletPhysicsEngine.h"
#include "CollisionCuboid.h"
#include "CollisionSphere.h"

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
}


BulletPhysicsEngine::~BulletPhysicsEngine()
{
}

btRigidBody* BulletPhysicsEngine::AddRigidBody(float mass, vec3 position, quat direction, CollisionShape * shape)
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

	dynamicsWorld->addRigidBody(rigidBody);

	return rigidBody;
}

vec3 BulletPhysicsEngine::GetPositionOfRigidBody(void * pRigidBody)
{
	btRigidBody * rigidBody = (btRigidBody* )pRigidBody;

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

void BulletPhysicsEngine::ApplyForce(void * pRigidBody, vec3 pForce)
{
	btRigidBody * rigidBody = (btRigidBody*)pRigidBody;

	btVector3 origin(0.0, 0.0, 0.0);
	btVector3 force(pForce.x, pForce.y, pForce.z);
	rigidBody->applyForce(force, origin);
}

void BulletPhysicsEngine::ApplyImpulse(void * pRigidBody, vec3 pImpulse)
{
	btRigidBody * rigidBody = (btRigidBody*)pRigidBody;

	btVector3 origin(0.0, 0.0, 0.0);
	btVector3 impulse(pImpulse.x, pImpulse.y, pImpulse.z);
	rigidBody->applyImpulse(impulse, origin);
}
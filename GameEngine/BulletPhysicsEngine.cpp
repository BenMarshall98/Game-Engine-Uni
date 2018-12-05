
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
	dynamicsWorld->setGravity(btVector3(0, -1.0f, 0));
}


BulletPhysicsEngine::~BulletPhysicsEngine()
{
}

btRigidBody* BulletPhysicsEngine::AddRigidBody(float mass, vec3 position, vec3 direction, float angle, CollisionShape * shape)
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

	quat directionQuat = angleAxis(radians(angle), direction);

	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(
		btQuaternion(directionQuat.x, directionQuat.y, directionQuat.z, directionQuat.w),
		btVector3(position.x, position.y, position.z)
	));

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, btVector3(0, 0, 0));

	btRigidBody * rigidBody = new btRigidBody(rigidBodyCI);

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

	return quat(rotation.x(), rotation.y(), rotation.z(), rotation.w());
}
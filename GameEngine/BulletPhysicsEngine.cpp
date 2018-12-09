
#include "BulletPhysicsEngine.h"
#include "CollisionCuboid.h"
#include "CollisionSphere.h"
#include "ComponentPhysics.h"

#include "glm/gtc/quaternion.hpp"

template<typename E>
BulletPhysicsEngine<E>::BulletPhysicsEngine()
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

template<typename E>
BulletPhysicsEngine<E>::~BulletPhysicsEngine()
{
}

template<typename E>
btRigidBody* BulletPhysicsEngine<E>::AddRigidBody(float mass, vec3 position, quat direction, CollisionShape * shape, ComponentPhysics<E> * componentPhysics)
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
	rigidBody->setUserPointer(componentPhysics);

	dynamicsWorld->addRigidBody(rigidBody);

	return rigidBody;
}

template<typename E>
vec3 BulletPhysicsEngine<E>::GetPositionOfRigidBody(void * pRigidBody)
{
	btRigidBody * rigidBody = (btRigidBody* )pRigidBody;

	rigidBody->activate();

	btTransform transform = rigidBody->getWorldTransform();
	btVector3 origin = transform.getOrigin();

	return vec3(origin.x(), origin.y(), origin.z());
}

template<typename E>
quat BulletPhysicsEngine<E>::GetDirectionOfRigidBody(void * pRigidBody)
{
	btRigidBody * rigidBody = (btRigidBody*)pRigidBody;

	btTransform transform = rigidBody->getWorldTransform();
	btQuaternion rotation = transform.getRotation();

	return quat(rotation.w(), rotation.x(), rotation.y(), rotation.z());
}

template<typename E>
void BulletPhysicsEngine<E>::ApplyVelocity(void * pRigidBody, vec3 pVelocity)
{
	btRigidBody * rigidBody = (btRigidBody*)pRigidBody;

	btVector3 currentVel = rigidBody->getLinearVelocity();
	vec3 currentVelocity(currentVel.x(), currentVel.y(), currentVel.z());
	vec3 desiredVelocity(pVelocity.x, currentVel.y(), pVelocity.z);

	vec3 velocity = mix(currentVelocity, desiredVelocity, 0.1);

	btVector3 vel(velocity.x, velocity.y, velocity.z);
	
	rigidBody->setLinearVelocity(vel);
}

template<typename E>
void BulletPhysicsEngine<E>::ApplyImpulse(void * pRigidBody, vec3 pImpulse)
{
	btRigidBody * rigidBody = (btRigidBody*)pRigidBody;

	btVector3 origin(0.0, 0.0, 0.0);
	btVector3 impulse(pImpulse.x, pImpulse.y, pImpulse.z);
	rigidBody->applyImpulse(impulse, origin);
}

template<typename E>
bool BulletPhysicsEngine<E>::collisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2)
{
	
	ComponentPhysics<E> * physicsComponent1 = (ComponentPhysics<E> *)((btRigidBody *)obj1)->getUserPointer();
	ComponentPhysics<E> * physicsComponent2 = (ComponentPhysics<E> *)((btRigidBody *)obj2)->getUserPointer();
}
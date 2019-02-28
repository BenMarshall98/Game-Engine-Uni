#include "BulletRigidBody.h"

BulletRigidBody::~BulletRigidBody()
{
}

vec3 BulletRigidBody::GetPosition()
{
	rigidBody->activate();

	btTransform transform = rigidBody->getWorldTransform();
	btVector3 origin = transform.getOrigin();

	return vec3(origin.x(), origin.y(), origin.z());
}

quat BulletRigidBody::GetDirection()
{
	btTransform transform = rigidBody->getWorldTransform();
	btQuaternion rotation = transform.getRotation();

	return quat(rotation.w(), rotation.x(), rotation.y(), rotation.z());
}

void * BulletRigidBody::GetRigidBody()
{
	return rigidBody;
}

void BulletRigidBody::ApplyVelocity(vec3 & pVelocity)
{
	btVector3 currentVel = rigidBody->getLinearVelocity();
	vec3 currentVelocity(currentVel.x(), currentVel.y(), currentVel.z());
	vec3 desiredVelocity(pVelocity.x, currentVel.y(), pVelocity.z);

	vec3 velocity = mix(currentVelocity, desiredVelocity, 0.1);

	btVector3 vel(velocity.x, velocity.y, velocity.z);

	rigidBody->setLinearVelocity(vel);
}

void BulletRigidBody::ApplyImpulse(vec3 & pImpulse)
{
	btVector3 origin(0.0, 0.0, 0.0);
	btVector3 impulse(pImpulse.x, pImpulse.y, pImpulse.z);
	rigidBody->applyImpulse(impulse, origin);
}

void BulletRigidBody::ApplyRotation(vec3 & pRotation)
{
	btVector3 currentRot = rigidBody->getAngularVelocity();
	vec3 currentRotation(currentRot.x(), currentRot.y(), currentRot.z());
	vec3 desiredRotation(pRotation.x, pRotation.y, pRotation.z);

	vec3 rotation = mix(currentRotation, desiredRotation, 0.1);

	btVector3 rot(rotation.x, rotation.y, rotation.z);

	rigidBody->setAngularVelocity(rot);
}
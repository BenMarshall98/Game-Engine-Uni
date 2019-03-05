#include "BulletRigidBody.h"
#include "PhysicsManager.h"

BulletRigidBody::~BulletRigidBody()
{
	PhysicsManager::Instance()->RemoveRigidBody(this);
	delete rigidBody;
}

glm::vec3 BulletRigidBody::GetPosition()
{
	rigidBody->activate();

	btTransform transform = rigidBody->getWorldTransform();
	btVector3 origin = transform.getOrigin();

	return glm::vec3(origin.x(), origin.y(), origin.z());
}

glm::quat BulletRigidBody::GetDirection()
{
	btTransform transform = rigidBody->getWorldTransform();
	btQuaternion rotation = transform.getRotation();

	return glm::quat(rotation.w(), rotation.x(), rotation.y(), rotation.z());
}

void * BulletRigidBody::GetRigidBody()
{
	return rigidBody;
}

void BulletRigidBody::ApplyVelocity(glm::vec3 & pVelocity)
{
	btVector3 currentVel = rigidBody->getLinearVelocity();
	glm::vec3 currentVelocity(currentVel.x(), currentVel.y(), currentVel.z());
	glm::vec3 desiredVelocity(pVelocity.x, currentVel.y(), pVelocity.z);

	glm::vec3 velocity = mix(currentVelocity, desiredVelocity, 0.1);

	btVector3 vel(velocity.x, velocity.y, velocity.z);

	rigidBody->setLinearVelocity(vel);
}

void BulletRigidBody::ApplyImpulse(glm::vec3 & pImpulse)
{
	btVector3 origin(0.0, 0.0, 0.0);
	btVector3 impulse(pImpulse.x, pImpulse.y, pImpulse.z);
	rigidBody->applyImpulse(impulse, origin);
}

void BulletRigidBody::ApplyRotation(glm::vec3 & pRotation)
{
	btVector3 currentRot = rigidBody->getAngularVelocity();
	glm::vec3 currentRotation(currentRot.x(), currentRot.y(), currentRot.z());
	glm::vec3 desiredRotation(pRotation.x, pRotation.y, pRotation.z);

	glm::vec3 rotation = mix(currentRotation, desiredRotation, 0.1);

	btVector3 rot(rotation.x, rotation.y, rotation.z);

	rigidBody->setAngularVelocity(rot);
}
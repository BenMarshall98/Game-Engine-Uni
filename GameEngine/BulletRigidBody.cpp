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
	const btVector3 origin = transform.getOrigin();

	return glm::vec3(origin.x(), origin.y(), origin.z());
}

glm::quat BulletRigidBody::GetDirection()
{
	const btTransform transform = rigidBody->getWorldTransform();
	const btQuaternion rotation = transform.getRotation();

	return glm::quat(rotation.w(), rotation.x(), rotation.y(), rotation.z());
}

void * BulletRigidBody::GetRigidBody()
{
	return rigidBody;
}

void BulletRigidBody::SetPosition(const glm::vec3 pPosition)
{
	const btVector3 origin(pPosition.x, pPosition.y, pPosition.z);

	btTransform transform = rigidBody->getWorldTransform();
	transform.setOrigin(origin);
	rigidBody->setWorldTransform(transform);
}

void BulletRigidBody::SetDirection(const glm::quat pRotation)
{
	const btQuaternion rotation(pRotation.x, pRotation.y, pRotation.z, pRotation.w);

	btTransform transform = rigidBody->getWorldTransform();
	transform.setRotation(rotation);
	rigidBody->setWorldTransform(transform);
}

void BulletRigidBody::ApplyVelocity(const glm::vec3 & pVelocity)
{
	const btVector3 currentVel = rigidBody->getLinearVelocity();
	const glm::vec3 currentVelocity(currentVel.x(), currentVel.y(), currentVel.z());
	const glm::vec3 desiredVelocity(pVelocity.x, currentVel.y(), pVelocity.z);

	const glm::vec3 velocity = mix(currentVelocity, desiredVelocity, 0.1);

	const btVector3 vel(velocity.x, velocity.y, velocity.z);

	rigidBody->setLinearVelocity(vel);
}

void BulletRigidBody::ApplyImpulse(const glm::vec3 & pImpulse)
{
	const btVector3 origin(0.0, 0.0, 0.0);
	const btVector3 impulse(pImpulse.x, pImpulse.y, pImpulse.z);
	rigidBody->applyImpulse(impulse, origin);
}

void BulletRigidBody::ApplyRotation(const glm::vec3 & pRotation)
{
	const btVector3 currentRot = rigidBody->getAngularVelocity();
	const glm::vec3 currentRotation(currentRot.x(), currentRot.y(), currentRot.z());
	const glm::vec3 desiredRotation(pRotation.x, pRotation.y, pRotation.z);

	const glm::vec3 rotation = mix(currentRotation, desiredRotation, 0.1);

	const btVector3 rot(rotation.x, rotation.y, rotation.z);

	rigidBody->setAngularVelocity(rot);
}
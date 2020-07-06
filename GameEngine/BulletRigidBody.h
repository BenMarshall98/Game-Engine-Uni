#pragma once
#include "RigidBody.h"
#include "BulletPhysics\btBulletDynamicsCommon.h"

#include "glm/gtc/quaternion.hpp"

class BulletRigidBody : public RigidBody
{
private:
	btRigidBody * rigidBody;
public:
	explicit BulletRigidBody(btRigidBody * const pRigidBody) : rigidBody(pRigidBody) {};
	~BulletRigidBody();

	BulletRigidBody& operator=(const BulletRigidBody&) = delete;
	BulletRigidBody(BulletRigidBody&) = delete;

	glm::vec3 GetPosition() override;
	glm::quat GetDirection() override;
	void * GetRigidBody() override;

	void SetPosition(const glm::vec3 & position) override;
	void SetDirection(const glm::quat & direction) override;

	void ApplyVelocity(const glm::vec3 & velocity) override;
	void ApplyImpulse(const glm::vec3 & impulse) override;
	void ApplyRotation(const glm::vec3 & rotation) override;
};


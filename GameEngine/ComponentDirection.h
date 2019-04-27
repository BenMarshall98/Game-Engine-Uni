#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

#include "glm/gtc/quaternion.hpp"

class ComponentDirection : public iComponent
{
private:
	glm::quat updateDirection;
	glm::quat renderDirection;

public:
	ComponentDirection(glm::vec3 & pDirection, float pAngle) :
		iComponent(ComponentType::COMPONENT_DIRECTION), updateDirection(glm::angleAxis(glm::radians(pAngle), pDirection)), renderDirection(angleAxis(pAngle, pDirection)) {}
	~ComponentDirection();

	void RenderSwap() override;

	//Gets the update thread direction
	inline const glm::quat & GetUpdateDirection() const
	{
		return updateDirection;
	}

	//Sets the update thread direction
	inline void SetUpdateDirection(const glm::quat & pDirection)
	{
		updateDirection = pDirection;
	}

	//Gets the render thread direction
	inline const glm::quat & GetRenderDirection() const
	{
		return renderDirection;
	}
};


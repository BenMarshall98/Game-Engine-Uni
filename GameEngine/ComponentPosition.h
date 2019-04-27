#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

class ComponentPosition : public iComponent
{
private:
	glm::vec3 updatePosition;
	glm::vec3 renderPosition;

public:
	explicit ComponentPosition(glm::vec3 & pPosition) :
		iComponent(ComponentType::COMPONENT_POSITION), updatePosition(pPosition), renderPosition(pPosition) {}
	~ComponentPosition();

	void RenderSwap() override;

	//Gets the update thread position
	inline const glm::vec3 & GetUpdatePosition() const
	{
		return updatePosition;
	}

	//Sets the update thread position
	inline void SetUpdatePosition(const glm::vec3 & pPosition)
	{
		updatePosition = pPosition;
	}

	//Gets the render thread position
	inline const glm::vec3 & GetRenderPosition() const
	{
		return renderPosition;
	}
};


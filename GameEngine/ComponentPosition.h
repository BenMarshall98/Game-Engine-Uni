#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

class ComponentPosition : public iComponent
{
private:
	glm::vec3 updatePosition;
	glm::vec3 renderPosition;

public:
	explicit ComponentPosition(glm::vec3 & pPosition) : updatePosition(pPosition), renderPosition(pPosition) {}
	~ComponentPosition();

	ComponentType GetComponentName() override;
	void RenderSwap() override;

	inline const glm::vec3 & GetUpdatePosition() const
	{
		return updatePosition;
	}

	inline void SetUpdatePosition(const glm::vec3 & pPosition)
	{
		updatePosition = pPosition;
	}

	inline const glm::vec3 & GetRenderPosition() const
	{
		return renderPosition;
	}
};


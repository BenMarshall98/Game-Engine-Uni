#pragma once

#include "glm/glm.hpp"

class Source
{
public:
	Source();
	~Source();

	virtual void SetPosition(glm::vec3 position) = 0;
	virtual void SetIsPaused(bool paused = true) = 0;
	virtual void SetPlayPosition(int position) = 0;
	virtual void SetIsLooped(bool looped) = 0;
	virtual bool GetIsPaused() = 0;
	virtual bool IsFinished() = 0;
};


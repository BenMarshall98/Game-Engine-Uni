#pragma once

#include "Buffer.h"
#include "Source.h"
#include <string>

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	virtual Buffer * GenerateBuffer(const std::string & fileName) = 0;
	
	//Generates source from buffer
	inline Source * GenerateSource(Buffer * buffer)
	{
		return buffer->GenerateSource();
	}

	virtual void DeleteBuffer(Buffer * const buffer) const = 0;
	virtual void DeleteSource(Source * const source) const = 0;

	virtual void Clear() = 0;

	virtual void SetListenerPosition(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up) = 0;

	virtual void PauseSounds(bool pauseState) = 0;
};


#pragma once

#pragma comment(lib, "../irrKlang-64bit-1.6.0/lib/Winx64-visualStudio/irrKlang.lib")

#include "irrKlang-64bit-1.6.0/include/irrKlang.h"
#include <string>
#include "glm/glm.hpp"
#include <vector>
#include "Camera.h"
#include "Entity.h"
#include "LocationSound.h"
#include "EntitySound.h"

enum class AudioPlayback
{
	PLAY,
	STOP,
	PAUSE,
	NONE
};

class AudioManager
{
private:
	std::vector<void *> cameraSounds;
	std::vector<LocationSound *> locationSounds;
	std::vector<EntitySound *> entitySounds;

	irrklang::ISoundEngine * engine;

	static AudioManager * instance;
	
	AudioManager();

public:
	static inline AudioManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new AudioManager();
		}
		return instance;
	}

	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	void Update();
	void Clear();

	void * GenerateBuffer(const std::string & fileName);
	void * GenerateSource(void * buffer);

	void DeleteBuffer(void * const buffer) const;
	void DeleteSource(void * const source) const;
	void UpdateComponentSound(void * const source, const glm::vec3 & position, AudioPlayback playback) const;

	void PlayAudio(const std::string & sound);
	void PlayAudioAtLocation(const std::string & sound, const glm::vec3 & location);
	void PlayAudioAtEntityLocation(const std::string & sound, Entity * const entity);
	~AudioManager();
};


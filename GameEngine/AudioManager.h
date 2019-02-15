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

using namespace irrklang;

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
	vector<void *> cameraSounds;
	vector<LocationSound *> locationSounds;
	vector<EntitySound *> entitySounds;

	ISoundEngine * engine;

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

	void * GenerateBuffer(const string & fileName);
	void * GenerateSource(void * buffer);

	void DeleteBuffer(void * const buffer) const;
	void DeleteSource(void * const source) const;
	void UpdateComponentSound(void * const source, const vec3 & position, AudioPlayback playback) const;

	void PlayAudio(const string & sound);
	void PlayAudioAtLocation(const string & sound, const vec3 & location);
	void PlayAudioAtEntityLocation(const string & sound, Entity * const entity);
	~AudioManager();
};


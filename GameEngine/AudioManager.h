#pragma once

#pragma comment(lib, "../irrKlang-64bit-1.6.0/lib/Winx64-visualStudio/irrKlang.lib")

#include "irrKlang-64bit-1.6.0/include/irrKlang.h"
#include <string>
#include "glm/glm.hpp"
#include <vector>
#include "Camera.h"
#include "Entity.h"

using namespace irrklang;

enum class AudioPlayback
{
	PLAY,
	STOP,
	PAUSE,
	NONE
};

struct LocationSound
{
	void * source;
	vec3 location;
};

struct EntitySound
{
	void * source;
	Entity * entity;
};

class AudioManager
{
private:
	ISoundEngine * engine;

	vector<void *> cameraSounds;
	vector<LocationSound *> locationSounds;
	vector<EntitySound *> entitySounds;

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

	void Update();

	void * GenerateBuffer(string fileName);
	void * GenerateSource(void * buffer);

	void DeleteBuffer(void * buffer);
	void DeleteSource(void * source);
	void UpdateComponentSound(void * source, const vec3 & position, AudioPlayback playback);

	void PlayAudio(string & sound);
	void PlayAudioAtLocation(string & sound, vec3 & location);
	void PlayAudioAtEntityLocation(string & sound, Entity * entity);
	~AudioManager();
};


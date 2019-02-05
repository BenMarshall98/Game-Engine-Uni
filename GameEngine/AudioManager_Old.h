#pragma once
#pragma comment(lib, "../OpenAL/libs/Win64/OpenAL32.lib")

#include <string>
#include "OpenAL/include/al.h"
#include "OpenAL/include/alc.h"
#include "glm/glm.hpp"
#include <vector>
#include "Camera.h"
#include "Entity.h"

using namespace std;

enum class AudioPlayback
{
	PLAY,
	STOP,
	PAUSE,
	NONE
};

struct LocationSound
{
	unsigned int source;
	vec3 location;
};

struct EntitySound
{
	unsigned int source;
	Entity * entity;
};

class AudioManager_Old
{
private:
	ALCdevice * device;
	ALCcontext * context;

	vector<unsigned int> cameraSounds;
	vector<LocationSound *> locationSounds;
	vector<EntitySound *> entitySounds;

	static AudioManager_Old * instance;
	static void LoadWAVFile(string & fileName, ALenum * channels, void ** data, unsigned int * size, unsigned int * frequency);
	AudioManager_Old();
public:
	
	static inline AudioManager_Old * Instance()
	{
		if (instance == nullptr)
		{
			instance = new AudioManager_Old();
		}

		return instance;
	}

	void Update();

	unsigned int GenerateBuffer(string fileName);
	unsigned int GenerateSource(unsigned int buffer);

	void DeleteSource(unsigned int source);
	void UpdateComponentSound(unsigned int source, const vec3 & position, AudioPlayback playback);

	void PlayAudio(string & sound);
	void PlayAudioAtLocation(string & sound, vec3 & location);
	void PlayAudioAtEntityLocation(string & sound, Entity * entity);
	~AudioManager_Old();
};


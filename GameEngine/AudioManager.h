#pragma once
#pragma comment(lib, "../OpenAL/libs/Win64/OpenAL32.lib")

#include <string>
#include "OpenAL/include/al.h"
#include "OpenAL/include/alc.h"
#include "glm/glm.hpp"
#include <vector>
#include "Camera.h"

using namespace std;

enum class AudioPlayback
{
	START,
	STOP,
	PAUSE,
	NONE
};

class AudioManager
{
private:
	ALCdevice * device;
	ALCcontext * context;

	static AudioManager * instance;
	static void LoadWAVFile(string & fileName, ALenum * channels, void * data, unsigned int * size, unsigned int * frequency);
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

	unsigned int GenerateBuffer(string fileName);
	unsigned int GenerateSource(unsigned int buffer);

	void DeleteSource(unsigned int source);
	void UpdateComponentSound(unsigned int source, const vec3 & position, AudioPlayback playback);
	~AudioManager();
};


#pragma once

#include <string>
#include "OpenAL/include/al.h"
#include "OpenAL/include/alc.h"
#include <vector>

using namespace std;

class AudioManager
{
private:
	ALCdevice * device;
	ALCcontext * context;

	vector<ALuint> buffers;
	vector<ALuint> sources;

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

	unsigned int GenerateBuffer(string fileName);
	//int GenerateSource(int buffer);
	~AudioManager();
};


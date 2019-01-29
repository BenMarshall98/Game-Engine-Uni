#pragma once

#include <string>
#include "OpenAL/include/al.h"
#include "OpenAL/include/alc.h"

using namespace std;

class AudioManager
{
private:
	ALCdevice * device;
	ALCcontext * context;

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

	int GenerateBuffer(string fileName);
	int GenerateSource(int buffer);
	~AudioManager();
};


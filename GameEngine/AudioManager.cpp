#include "AudioManager.h"

AudioManager * AudioManager::instance = nullptr;

AudioManager::AudioManager()
{
	device = alcOpenDevice(0);

	if (device)
	{
		context = alcCreateContext(device, 0);
		alcMakeContextCurrent(context);
	}

	alGetError();
}


AudioManager::~AudioManager()
{
	alcMakeContextCurrent(0);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

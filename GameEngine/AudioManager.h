#pragma once
#pragma comment(lib, "../OpenAL/libs/Win64/OpenAL32.lib")

#include <string>
#include "OpenAL/include/al.h"
#include "OpenAL/include/alc.h"
#include <vector>
#include "Camera.h"

using namespace std;

class AudioManager
{
private:
	ALCdevice * device;
	ALCcontext * context;

	vector<ALuint> buffers;
	vector<ALuint> sources;

	Camera * mCamera;

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

	inline void SetCamera(Camera * pCamera)
	{
		mCamera = pCamera;
	}

	unsigned int GenerateBuffer(string fileName);
	unsigned int GenerateSource(unsigned int buffer);
	~AudioManager();
};


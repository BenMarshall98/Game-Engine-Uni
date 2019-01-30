#include "AudioManager.h"
#include <fstream>

AudioManager * AudioManager::instance = nullptr;

AudioManager::AudioManager() : mCamera(nullptr)
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

unsigned int AudioManager::GenerateBuffer(string fileName)
{
	ALuint buffer;

	alGenBuffers(1, &buffer);

	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		//TODO: Log error
		alDeleteBuffers(1, &buffer);
		return 0;
	}

	ALenum channels;
	void * data = nullptr;
	unsigned int size;
	unsigned int frequency;

	LoadWAVFile(fileName, &channels, data, &size, &frequency);

	alBufferData(buffer, channels, data, size, frequency);

	if ((error = alGetError()) != AL_NO_ERROR)
	{
		//TODO: Log error
		delete data;
		alDeleteBuffers(1, &buffer);
		return 0;
	}

	delete data;

	buffers.push_back(buffer);
	return buffer;
}

unsigned int AudioManager::GenerateSource(unsigned int buffer)
{
	unsigned int source;

	ALenum error;
	alGenSources(1, &source);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		//TODO: Log error
		alDeleteSources(1, &source);
		return 0;
	}

	alSourcei(source, AL_BUFFER, buffer);

	if ((error = alGetError()) != AL_NO_ERROR)
	{
		//TODO: Log error
		alDeleteSources(1, &source);
		return 0;
	}

	sources.push_back(source);
	return source;
}

void AudioManager::LoadWAVFile(string & fileName, ALenum * channels, void * allData, unsigned int * size, unsigned int * frequency)
{
	ifstream file(fileName, ios::binary);

	if (file.fail())
	{
		//TODO: log error opening file
		return;
	}

	char riff[4];
	file >> riff;

	if (riff != "RIFF")
	{
		//TODO: log error with file, not RIFF format
		return;
	}

	unsigned int fileSize;
	char wave[4];
	file >> fileSize >> wave;

	if (wave != "WAVE")
	{
		//TODO: log error with file, not WAVE file
		return;
	}

	char fmt[4];
	file >> fmt;

	if (fmt != "fmt ")
	{
		//TODO: log error with file, no data format
		return;
	}

	unsigned int fmtChunkSize, avgBytesPerSec;
	unsigned short formatTag, blockAlign, bitsPerSample;

	file >> fmtChunkSize >> formatTag >> *channels >> *frequency >> avgBytesPerSec >> blockAlign >> bitsPerSample;

	if (fmtChunkSize != 16)
	{
		//TODO: Log that fmt section of the file is wrong size;
		return;
	}

	if (*channels != 1)
	{
		//TODO: Log that there should only be one channel;
		return;
	}

	char data[4];
	file >> data;

	if (data != "data")
	{
		//TODO: log error with file, no data in file
		return;
	}

	file >> *size;

	if (bitsPerSample == 8)
	{
		allData = new char[*size];
		file >> allData;
		*channels = AL_FORMAT_MONO8;
	}
	else if (bitsPerSample == 16)
	{
		allData = new short[*size];
		file >> allData;
		*channels = AL_FORMAT_MONO16;
	}
}

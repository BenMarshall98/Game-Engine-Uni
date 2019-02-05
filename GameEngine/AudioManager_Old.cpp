#include "AudioManager_Old.h"
#include "CameraManager.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "ComponentPosition.h"
#include <fstream>

AudioManager_Old * AudioManager_Old::instance = nullptr;

AudioManager_Old::AudioManager_Old()
{
	device = alcOpenDevice(0);

	if (device)
	{
		context = alcCreateContext(device, 0);
		alcMakeContextCurrent(context);
	}

	alGetError();
}

AudioManager_Old::~AudioManager_Old()
{
	alcMakeContextCurrent(0);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

unsigned int AudioManager_Old::GenerateBuffer(string fileName)
{
	alGetError();
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

	LoadWAVFile(fileName, &channels, &data, &size, &frequency);

	alBufferData(buffer, channels, data, size, frequency);

	if ((error = alGetError()) != AL_NO_ERROR)
	{
		//TODO: Log error
		delete data;
		alDeleteBuffers(1, &buffer);
		return 0;
	}

	delete data;
	return buffer;
}

unsigned int AudioManager_Old::GenerateSource(unsigned int buffer)
{
	alGetError();
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
	return source;
}

void AudioManager_Old::LoadWAVFile(string & fileName, ALenum * channels, void ** allData, unsigned int * size, unsigned int * frequency)
{
	ifstream file(fileName, ios::binary);

	if (file.fail())
	{
		//TODO: log error opening file
		return;
	}

	char riff[5] = "";
	file.read(riff, 4);

	if (strcmp(riff, "RIFF") != 0)
	{
		//TODO: log error with file, not RIFF format
		return;
	}

	long fileSize;
	char wave[5] = "";
	file.read(reinterpret_cast<char *>(&fileSize), sizeof(long));
	file.read(wave, 4);

	if (strcmp(wave, "WAVE") != 0)
	{
		//TODO: log error with file, not WAVE file
		return;
	}

	char fmt[5] = "";
	file.read(fmt, 4);

	if (strcmp(fmt, "fmt ") != 0)
	{
		//TODO: log error with file, no data format
		return;
	}

	long fmtChunkSize, avgBytesPerSec, mFrequency;
	short mChannels, formatTag, blockAlign, bitsPerSample;

	file.read(reinterpret_cast<char *>(&fmtChunkSize), sizeof(long));
	file.read(reinterpret_cast<char *>(&formatTag), sizeof(short));
	file.read(reinterpret_cast<char *>(&mChannels), sizeof(short));
	file.read(reinterpret_cast<char *>(&mFrequency), sizeof(long));
	file.read(reinterpret_cast<char *>(&avgBytesPerSec), sizeof(long));
	file.read(reinterpret_cast<char *>(&blockAlign), sizeof(short));
	file.read(reinterpret_cast<char *>(&bitsPerSample), sizeof(short));

	if (fmtChunkSize != 16)
	{
		//TODO: Log that fmt section of the file is wrong size;
		return;
	}

	if (mChannels != 1)
	{
		//TODO: Log that there should only be one channel;
		return;
	}

	char data[5] = "";
	file.read(data, 4);

	if (strcmp(data, "data") != 0)
	{
		//TODO: log error with file, no data in file
		return;
	}

	long mSize;
	file.read(reinterpret_cast<char *>(&mSize), sizeof(long));

	*frequency = mFrequency;
	*size = mSize;

	if (bitsPerSample == 8)
	{
		char * data = new char[mSize];
		file >> data;
		*allData = data;
		*channels = AL_FORMAT_MONO8;
	}
	else if (bitsPerSample == 16)
	{
		short * data = new short[mSize];
		file.read(reinterpret_cast<char *>(data), sizeof(short) * mSize);
		*allData = data;
		*channels = AL_FORMAT_MONO16;
	}
}

void AudioManager_Old::UpdateComponentSound(unsigned int source, const vec3 & position, AudioPlayback playback)
{
	alSource3f(source, AL_POSITION, position.x, position.y, position.z);

	if (playback == AudioPlayback::PAUSE)
	{
		alSourcePause(source);
	}
	else if (playback == AudioPlayback::STOP)
	{
		alSourceStop(source);
	}
	else if (playback == AudioPlayback::PLAY)
	{
		int value;
		alGetSourcei(source, AL_SOURCE_STATE, &value);
		if (value != AL_PLAYING)
		{
			alSourcePlay(source);
			alSourcei(source, AL_LOOPING, AL_TRUE);
		}
	}
}

void AudioManager_Old::Update()
{
	Camera * camera = CameraManager::Instance()->GetCamera();

	vec3 pos = camera->GetPosition();
	vec3 up = camera->GetUp();
	vec3 lookAt = camera->GetLookAt();

	float position[] = { pos.x, pos.y, pos.z };
	float orientation[] = { lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z };

	alListenerfv(AL_POSITION, position);
	alListenerfv(AL_ORIENTATION, orientation);

	for (int i = 0; i < cameraSounds.size(); i++)
	{
		ALenum playing;
		alGetSourcei(cameraSounds.at(i), AL_SOURCE_STATE, &playing);

		if (playing == AL_STOPPED)
		{
			DeleteSource(cameraSounds.at(i));
			cameraSounds.erase(cameraSounds.begin() + i);
			i--;
			continue;
		}

		alSource3f(cameraSounds.at(i), AL_POSITION, pos.x, pos.y, pos.z);
	}

	for (int i = 0; i < locationSounds.size(); i++)
	{
		ALenum playing;
		alGetSourcei(locationSounds.at(i)->source, AL_SOURCE_STATE, &playing);

		if (playing == AL_STOPPED)
		{
			DeleteSource(locationSounds.at(i)->source);
			delete locationSounds.at(i);
			locationSounds.erase(locationSounds.begin() + i);
			i--;
			continue;
		}
	}

	for (int i = 0; i < entitySounds.size(); i++)
	{
		ALenum playing;
		alGetSourcei(entitySounds.at(i)->source, AL_SOURCE_STATE, &playing);

		if (playing == AL_STOPPED)
		{
			DeleteSource(entitySounds.at(i)->source);
			delete entitySounds.at(i);
			entitySounds.erase(entitySounds.begin() + i);
			i--;
			continue;
		}

		iComponent * componentPosition = EntityManager::Instance()->GetComponentOfEntity(entitySounds.at(i)->entity, ComponentType::COMPONENT_POSITION);

		vec3 entPos = dynamic_cast<ComponentPosition *>(componentPosition)->GetUpdatePosition();

		alSource3f(entitySounds.at(i)->source, AL_POSITION, entPos.x, entPos.y, entPos.z);
	}
}

void AudioManager_Old::PlayAudio(string & sound)
{
	unsigned int source = ResourceManager::GetAudio(sound);
	alSourcePlay(source);
	alSourcei(source, AL_LOOPING, AL_FALSE);

	Camera * camera = CameraManager::Instance()->GetCamera();

	vec3 pos = camera->GetPosition();
	alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);

	cameraSounds.push_back(source);
}

void AudioManager_Old::PlayAudioAtLocation(string & sound, vec3 & location)
{
	unsigned int source = ResourceManager::GetAudio(sound);
	alSourcePlay(source);
	alSourcei(source, AL_LOOPING, AL_FALSE);

	alSource3f(source, AL_POSITION, location.x, location.y, location.z);

	LocationSound * locSound = new LocationSound();
	locSound->location = location;
	locSound->source = source;

	locationSounds.push_back(locSound);
}

void AudioManager_Old::PlayAudioAtEntityLocation(string & sound, Entity * entity)
{
	unsigned int source = ResourceManager::GetAudio(sound);
	alSourcePlay(source);
	alSourcei(source, AL_LOOPING, AL_FALSE);

	iComponent * componentPosition = EntityManager::Instance()->GetComponentOfEntity(entity, ComponentType::COMPONENT_POSITION);

	vec3 pos = dynamic_cast<ComponentPosition *>(componentPosition)->GetUpdatePosition();

	alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);

	EntitySound * entSound = new EntitySound();
	entSound->entity = entity;
	entSound->source = source;

	entitySounds.push_back(entSound);
}

void AudioManager_Old::DeleteSource(unsigned int source)
{
	alSourceStop(source);
	alDeleteSources(1, &source);
}

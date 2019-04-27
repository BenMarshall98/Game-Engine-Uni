#pragma once

#include <string>
#include "glm/glm.hpp"
#include <vector>
#include "Camera.h"
#include "Entity.h"
#include "LocationSound.h"
#include "EntitySound.h"

#include "AudioEngine.h"
#include "irrKlangEngine.h"

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
	std::vector<Source *> cameraSounds;
	std::vector<LocationSound *> locationSounds;
	std::vector<EntitySound *> entitySounds;

	static AudioEngine * audioEngine;

	static std::string audioName;

	static AudioManager * instance;
	
	AudioManager();

public:

	//Sets the audio engine to be used
	static void InitAudioEngineName(std::string pAudioName)
	{
		audioName = pAudioName;
	}

	static inline AudioManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new AudioManager();
		}
		if (audioEngine == nullptr)
		{
			if (audioName == "irrKlang")
			{
				audioEngine = new irrKlangEngine();
			}
		}
		return instance;
	}

	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	void Update();

	//Clears the audio engine between scenes
	inline void Clear()
	{
		audioEngine->Clear();
	}

	//Generates a buffer from audio file
	inline Buffer * GenerateBuffer(const std::string & fileName)
	{
		return audioEngine->GenerateBuffer(fileName);
	}

	//Generates a source from buffer
	inline Source * GenerateSource(Buffer * buffer)
	{
		return audioEngine->GenerateSource(buffer);
	}

	//Deletes buffer
	inline void DeleteBuffer(Buffer * const buffer) const
	{
		audioEngine->DeleteBuffer(buffer);
	}


	//Deletes source
	inline void DeleteSource(Source * const source) const
	{
		audioEngine->DeleteSource(source);
	}

	void UpdateComponentSound(Source * const source, const glm::vec3 & position, AudioPlayback playback) const;
	void PlayAudio(const std::string & sound);
	void PlayAudioAtLocation(const std::string & sound, const glm::vec3 & location);
	void PlayAudioAtEntityLocation(const std::string & sound, Entity * const entity);
	~AudioManager();

	//Pauses all sounds
	inline void PauseSounds(bool pauseState)
	{
		audioEngine->PauseSounds(pauseState);
	}
};


#include "AudioManager.h"
#include "CameraManager.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "ComponentPosition.h"
#include <fstream>

AudioManager * AudioManager::instance = nullptr;

AudioManager::AudioManager()
{
	engine = createIrrKlangDevice();
}


AudioManager::~AudioManager()
{
	engine->drop();
}

void * AudioManager::GenerateBuffer(string fileName)
{
	ISoundSource * buffer = engine->addSoundSourceFromFile(fileName.c_str());

	return buffer;
}

void * AudioManager::GenerateSource(void * pBuffer)
{
	ISoundSource * mBuffer = (ISoundSource *)(pBuffer);
	ISound * source = engine->play3D(mBuffer, vec3df(0, 0, 0), false, true, true);

	return source;
}

void AudioManager::UpdateComponentSound(void * pSource, const vec3 & position, AudioPlayback playback)
{
	ISound * mSource = (ISound *)(pSource);

	if (!mSource)
	{
		return;
	}
	
	mSource->setPosition(vec3df(position.x, position.y, position.z));

	if (playback == AudioPlayback::PAUSE)
	{
		mSource->setIsPaused();
	}
	else if (playback == AudioPlayback::STOP)
	{
		mSource->setIsPaused();
		mSource->setPlayPosition(0);
	}
	else if (playback == AudioPlayback::PLAY)
	{
		if (mSource->getIsPaused())
		{
			mSource->setIsPaused(false);
			mSource->setIsLooped(true);
		}
	}
}

void AudioManager::Update()
{
	Camera * camera = CameraManager::Instance()->GetCamera();

	vec3 pos = camera->GetPosition();
	vec3 up = camera->GetUp();
	vec3 lookAt = camera->GetLookAt();

	engine->setListenerPosition(vec3df(pos.x, pos.y, pos.z), vec3df(lookAt.x, lookAt.y, lookAt.z),
		vec3df(0, 0, 0), vec3df(up.x, up.y, up.z));

	for (int i = 0; i < cameraSounds.size(); i++)
	{
		ISound * source = (ISound *)(cameraSounds.at(i));

		if (source->isFinished())
		{
			DeleteSource(source);
			cameraSounds.erase(cameraSounds.begin() + i);
			i--;
			continue;
		}

		source->setPosition(vec3df(pos.x, pos.y, pos.z));
	}

	for (int i = 0; i < locationSounds.size(); i++)
	{
		ISound * source = (ISound *)(locationSounds.at(i)->source);
		
		if (source->isFinished())
		{
			DeleteSource(source);
			delete locationSounds.at(i);
			locationSounds.erase(locationSounds.begin() + i);
			i--;
			continue;
		}
	}

	for (int i = 0; i < entitySounds.size(); i++)
	{
		ISound * source = (ISound *)(entitySounds.at(i)->source);

		if (source->isFinished())
		{
			DeleteSource(source);
			delete entitySounds.at(i);
			entitySounds.erase(entitySounds.begin() + i);
			i--;
			continue;
		}

		iComponent * componentPosition = EntityManager::Instance()->GetComponentOfEntity(entitySounds.at(i)->entity, ComponentType::COMPONENT_POSITION);

		vec3 entPos = dynamic_cast<ComponentPosition *>(componentPosition)->GetUpdatePosition();

		source->setPosition(vec3df(entPos.x, entPos.y, entPos.z));
	}
}

void AudioManager::PlayAudio(string & sound)
{
	void * preSource = ResourceManager::GetAudio(sound);

	ISound * source = (ISound *)preSource;

	source->setIsPaused(false);

	Camera * camera = CameraManager::Instance()->GetCamera();

	vec3 pos = camera->GetPosition();

	source->setPosition(vec3df(pos.x, pos.y, pos.z));

	cameraSounds.push_back(source);
}

void AudioManager::PlayAudioAtLocation(string & sound, vec3 & location)
{
	void * preSource = ResourceManager::GetAudio(sound);

	ISound * source = (ISound *)preSource;

	source->setIsPaused(false);

	source->setPosition(vec3df(location.x, location.y, location.z));

	LocationSound * locSound = new LocationSound();
	locSound->location = location;
	locSound->source = source;

	locationSounds.push_back(locSound);
}

void AudioManager::PlayAudioAtEntityLocation(string & sound, Entity * entity)
{
	void * preSource = ResourceManager::GetAudio(sound);

	ISound * source = (ISound *)preSource;
	
	source->setIsPaused(false);

	iComponent * componentPosition = EntityManager::Instance()->GetComponentOfEntity(entity, ComponentType::COMPONENT_POSITION);

	vec3 pos = dynamic_cast<ComponentPosition * >(componentPosition)->GetUpdatePosition();

	source->setPosition(vec3df(pos.x, pos.y, pos.z));

	EntitySound * entSound = new EntitySound();
	entSound->entity = entity;
	entSound->source = source;
	
	entitySounds.push_back(entSound);
}

void AudioManager::DeleteSource(void * pSource)
{
	ISound * mSource = (ISound *)pSource;
	mSource->stop();
	mSource->drop();
}
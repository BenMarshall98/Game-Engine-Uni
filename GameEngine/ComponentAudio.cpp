#include "ComponentAudio.h"
#include "ResourceManager.h"

ComponentAudio::ComponentAudio(AudioPlayback pPlayback, string pAudioName) : mPlayback(pPlayback)
{
	mSource = ResourceManager::GetAudio(pAudioName);
}

ComponentAudio::~ComponentAudio()
{
	AudioManager::Instance()->DeleteSource(mSource);
}

ComponentType ComponentAudio::GetComponentName()
{
	return ComponentType::COMPONENT_AUDIO;
}

void ComponentAudio::RenderSwap()
{

}
#include "ComponentAudio.h"
#include "ResourceManager.h"

ComponentAudio::ComponentAudio(const AudioPlayback pPlayback, const std::string & pAudioName) :
	iComponent(ComponentType::COMPONENT_AUDIO), mPlayback(pPlayback), mSource(ResourceManager::GetAudio(pAudioName))
{
}

ComponentAudio::~ComponentAudio()
{
	AudioManager::Instance()->DeleteSource(mSource);
}

void ComponentAudio::RenderSwap()
{

}
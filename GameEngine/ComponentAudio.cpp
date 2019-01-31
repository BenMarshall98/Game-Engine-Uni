#include "ComponentAudio.h"
#include "ResourceManager.h"

ComponentAudio::ComponentAudio(AudioPlayback pPlayback, string pAudioName) : mPlayback(pPlayback)
{
	mSource = ResourceManager::GetAudio(pAudioName);
}


ComponentAudio::~ComponentAudio()
{
}

#pragma once

#include "iComponent.h"
#include "AudioManager.h"
#include <string>

using namespace std;



class ComponentAudio : public iComponent
{
private:
	AudioPlayback mPlayback;
	unsigned int mSource;

public:
	ComponentAudio(AudioPlayback pPlayback, string audioName);
	~ComponentAudio();

	ComponentAudio& operator=(const ComponentAudio&) = delete;
	ComponentAudio(ComponentAudio&) = delete;

	ComponentType GetComponentName() override;

	void RenderSwap() override;

	inline AudioPlayback GetUpdateAudioPlayback()
	{
		return mPlayback;
	}

	inline void SetUpdateAudioPlayback(AudioPlayback pPlayback)
	{
		mPlayback = pPlayback;
	}

	inline unsigned int GetUpdateAudioSource()
	{
		return mSource;
	}
};


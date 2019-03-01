#pragma once

#include "iComponent.h"
#include "AudioManager.h"
#include <string>

class ComponentAudio : public iComponent
{
private:
	AudioPlayback mPlayback;
	void * mSource;

public:
	ComponentAudio(const AudioPlayback pPlayback, const std::string & audioName);
	~ComponentAudio();

	ComponentAudio& operator=(const ComponentAudio&) = delete;
	ComponentAudio(ComponentAudio&) = delete;

	ComponentType GetComponentName() override;

	void RenderSwap() override;

	inline AudioPlayback GetUpdateAudioPlayback() const
	{
		return mPlayback;
	}

	inline void SetUpdateAudioPlayback(const AudioPlayback pPlayback)
	{
		mPlayback = pPlayback;
	}

	inline void * GetUpdateAudioSource() const
	{
		return mSource;
	}
};


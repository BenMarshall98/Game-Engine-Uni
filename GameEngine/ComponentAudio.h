#pragma once

#include "iComponent.h"
#include "AudioManager.h"
#include <string>

class ComponentAudio : public iComponent
{
private:
	AudioPlayback mPlayback;
	Source * mSource;

public:
	ComponentAudio(const AudioPlayback pPlayback, const std::string & audioName);
	~ComponentAudio();

	ComponentAudio& operator=(const ComponentAudio&) = delete;
	ComponentAudio(ComponentAudio&) = delete;

	void RenderSwap() override;

	inline AudioPlayback GetUpdateAudioPlayback() const
	{
		return mPlayback;
	}

	inline void SetUpdateAudioPlayback(const AudioPlayback pPlayback)
	{
		mPlayback = pPlayback;
	}

	inline Source * GetUpdateAudioSource() const
	{
		return mSource;
	}
};


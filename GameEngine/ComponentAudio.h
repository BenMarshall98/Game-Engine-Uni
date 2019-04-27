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

	//Gets the audio playback state
	inline AudioPlayback GetUpdateAudioPlayback() const
	{
		return mPlayback;
	}

	//Sets the audio playback state
	inline void SetUpdateAudioPlayback(const AudioPlayback pPlayback)
	{
		mPlayback = pPlayback;
	}

	//Gets the audio source
	inline Source * GetUpdateAudioSource() const
	{
		return mSource;
	}
};


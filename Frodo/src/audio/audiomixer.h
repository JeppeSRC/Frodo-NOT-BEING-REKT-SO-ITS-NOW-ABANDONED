#pragma once

#include "audiomanager.h"
#include <util/list.h>


namespace FD {

class FDAPI AudioMixer {
private:
	IXAudio2SubmixVoice* mixer;
	List<XAUDIO2_SEND_DESCRIPTOR> outputs;

public:
	AudioMixer();
	~AudioMixer();

	IXAudio2SubmixVoice* GetMixer() const { return mixer; }

	uint32 GetNumChannels() const;
	uint32 GetSampleRate() const;
	float GetVolume() const;
	void GetVolume(float* volumes) const;

	void SetVolume(float vol);
	void SetVolume(float* volumes);

	void AddOutput(AudioMixer* mixer);

	void ClearOutputs();
};

}
#pragma once

#include <xaudio2.h>

namespace FD {

class AudioDevice {
private:
	IXAudio2MasteringVoice* device;

	uint32 channels;

public:
	AudioDevice();
	~AudioDevice();

	inline IXAudio2MasteringVoice* GetDevice() const { return device; }
	inline uint32 GetNumChannels() const { return channels; }
};

}

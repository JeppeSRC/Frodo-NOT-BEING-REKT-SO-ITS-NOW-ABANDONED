#pragma once

#include <xaudio2.h>

namespace FD {

class AudioDevice {
private:
	IXAudio2MasteringVoice* device;

public:
	AudioDevice();
	~AudioDevice();

	inline IXAudio2MasteringVoice* GetDevice() const { return device; }
};

}

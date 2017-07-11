#pragma once

#include <xaudio2.h>
#include <fd.h>

namespace FD {

class FDAPI AudioDevice {
private:
	IXAudio2MasteringVoice* device;

public:
	AudioDevice();
	~AudioDevice();

	inline IXAudio2MasteringVoice* GetDevice() const { return device; }
	
	inline uint32 GetNumChannels() const;
	uint32 GetChannelMask() const;
	float GetVolume() const;
	void GetVolume(float* volumes) const;
	uint32 GetSampleRate() const;

	void SetVolume(float vol);
	void SetVolume(float* volumes);
};

}

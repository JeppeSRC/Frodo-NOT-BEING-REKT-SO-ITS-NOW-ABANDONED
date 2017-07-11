#include "xadevice.h"
#include "audiomanager.h"
#include <core/log.h>

namespace FD {

AudioDevice::AudioDevice() {
	HRESULT res = AudioManager::engine->CreateMasteringVoice(&device);
	
	if (res) {
		FD_FATAL("No audio device available!");
		return;
	}
}

AudioDevice::~AudioDevice() {
	if (device) device->DestroyVoice();
}

uint32 AudioDevice::GetNumChannels() const {
	XAUDIO2_VOICE_DETAILS d;
	device->GetVoiceDetails(&d);
	return d.InputChannels;
}

uint32 AudioDevice::GetChannelMask() const {
	uint32 mask;
	device->GetChannelMask((DWORD*)&mask);
	return mask;
}

float AudioDevice::GetVolume() const {
	float vol;
	device->GetVolume(&vol);
	return vol;
}

void AudioDevice::GetVolume(float* volumes) const {
	device->GetChannelVolumes(GetNumChannels(), volumes);
}

void AudioDevice::SetVolume(float vol) {
	device->SetVolume(vol);
}

void AudioDevice::SetVolume(float* volumes) {
	device->SetChannelVolumes(GetNumChannels(), volumes);
}

uint32 AudioDevice::GetSampleRate() const {
	XAUDIO2_VOICE_DETAILS d;
	device->GetVoiceDetails(&d);
	return d.InputSampleRate;
}

}

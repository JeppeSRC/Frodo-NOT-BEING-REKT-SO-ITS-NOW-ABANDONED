#include "audiomixer.h"
#include <core/log.h>

namespace FD {

AudioMixer::AudioMixer() {
	HRESULT res = AudioManager::engine->CreateSubmixVoice(&mixer, AudioManager::device->GetNumChannels(), AudioManager::device->GetSampleRate());

	if (res) {
		FD_FATAL("Unable to create submixer");
	}
}

AudioMixer::~AudioMixer() {
	if (mixer) mixer->DestroyVoice();
}

uint32 AudioMixer::GetNumChannels() const {
	XAUDIO2_VOICE_DETAILS d;
	mixer->GetVoiceDetails(&d);
	return d.InputChannels;
}

uint32 AudioMixer::GetSampleRate() const {
	XAUDIO2_VOICE_DETAILS d;
	mixer->GetVoiceDetails(&d);
	return d.InputSampleRate;
}

float AudioMixer::GetVolume() const {
	float vol;
	mixer->GetVolume(&vol);
	return vol;
}

void AudioMixer::GetVolume(float* volumes) const {
	mixer->GetChannelVolumes(GetNumChannels(), volumes);
}

void AudioMixer::SetVolume(float vol) {
	mixer->SetVolume(vol);
}

void AudioMixer::SetVolume(float* volumes) {
	mixer->SetChannelVolumes(GetNumChannels(), volumes);
}

void AudioMixer::AddOutput(AudioMixer* mixer) {
	outputs.Push_back({ 0, mixer->mixer });
	XAUDIO2_VOICE_SENDS sends;

	sends.SendCount = outputs.GetSize();
	sends.pSends = outputs.GetData();

	this->mixer->SetOutputVoices(&sends);
}

void AudioMixer::ClearOutputs() {
	outputs.Clear();
	mixer->SetOutputVoices(nullptr);
}

}
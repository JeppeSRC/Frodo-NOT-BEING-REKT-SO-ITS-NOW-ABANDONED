#include "audiomanager.h"

#include <core/log.h>

namespace FD {

IXAudio2* AudioManager::engine = nullptr;
AudioDevice* AudioManager::device = nullptr;

bool AudioManager::Init() {
	HRESULT res = XAudio2Create(&engine);

	if (!res) {
		FD_FATAL("Failed to intialize XAudio");
		return false;
	}

	return true;
}


void AudioManager::Release() {
	FD_ASSERT(engine == nullptr);
	engine->Release();
	
	engine = nullptr;
}

AudioDevice* AudioManager::CreateDevice() {
	if (device) {
		delete device;
	}

	AudioDevice* dev = new AudioDevice;

	device = dev;

	return dev;
}

}

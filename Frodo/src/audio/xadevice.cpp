#include "xadevice.h"
#include "audiomanager.h"
#include <core/log.h>

namespace FD {

AudioDevice::AudioDevice() {
	HRESULT res = AudioManager::Get()->CreateMasteringVoice(&device);
	
	if (!res) {
		FD_FATAL("No audio device available!");
	}
}

AudioDevice::~AudioDevice() {
	device->DestroyVoice();
}

}

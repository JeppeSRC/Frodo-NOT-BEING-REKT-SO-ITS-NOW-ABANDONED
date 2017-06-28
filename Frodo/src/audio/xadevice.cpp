#include "xadevice.h"
#include "audio.h"
#include <core/log.h>

namespace FD {

AudioDevice::AudioDevice() {
	HRESULT res = Audio::Get()->CreateMasteringVoice(&device);
	
	if (!res) {
		FD_FATAL("No audio device available!");
	}
}

AudioDevice::~AudioDevice() {
	device->DestroyVoice();
}

}

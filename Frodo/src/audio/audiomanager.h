#pragma once

#include <xaudio2.h>
#include <x3daudio.h>
#include "xadevice.h"

namespace FD {

class AudioManager {
private:
	static IXAudio2* engine;
	static AudioDevice* device;

public:
	static bool Init();
	static void Release();

	static AudioDevice* CreateDevice();

	inline static IXAudio2* Get() { return engine; }
	inline static AudioDevice* GetDevice() { return device; }

};

}

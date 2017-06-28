#pragma once

#include <xaudio2.h>
#include "xadevice.h"

namespace FD {

class Audio {
private:
	static IXAudio2* engine;

public:
	static bool Init();
	static void Dispose();

	inline static IXAudio2* Get() { return engine; }

};

}

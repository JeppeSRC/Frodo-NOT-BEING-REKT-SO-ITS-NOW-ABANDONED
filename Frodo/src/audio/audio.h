#pragma once

#include <fd.h>
#include <util/string.h>
#include <util/wave.h>
#include "audiomanager.h"
#include "audiomixer.h"

namespace FD {

class FDAPI Audio {
private:
	WAVE* wave;
	IXAudio2SourceVoice* sourceVoice;

	List<XAUDIO2_SEND_DESCRIPTOR> outputs;

	X3DAUDIO_EMITTER emitter;

public:
	Audio(const String& filename);
	~Audio();

	void Update();
	void Update(vec3 position, vec3 velocity, vec3 forward, vec3 up);
	void UpdatePosition(vec3 position, vec3 velocity);
	void UpdateOrientation(vec3 forward, vec3 up);

	void Play() const;
	void Stop() const;
	
	void AddOutput(AudioMixer* mixer);

	void ClearOutputs();
};

}

#include "audio.h"
#include <fdutils.h>
#include <core/log.h>

namespace FD {

Audio::Audio(const String& filename) {
	sourceVoice = nullptr;
	wave = FDReadWaveFile(filename);

	if (!wave) {
		FD_FATAL("It's wrong ya donk!");
		return;
	}

	XAUDIO2_BUFFER buffer = { 0 };
	WAVEFORMATEX fmt = { 0 };

	fmt.wFormatTag = WAVE_FORMAT_PCM;
	fmt.nChannels = wave->format.NumChannels;
	fmt.nSamplesPerSec = wave->format.SampleRate;
	fmt.nAvgBytesPerSec = wave->format.ByteRate;
	fmt.nBlockAlign = wave->format.BlockAlign;
	fmt.wBitsPerSample = wave->format.BitsPerSample;

	buffer.AudioBytes = wave->data.SubChunkSize;
	buffer.pAudioData = wave->audioData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	HRESULT res = AudioManager::engine->CreateSourceVoice(&sourceVoice, &fmt);

	if (res) {
		FD_FATAL("Failed to create source voice");
	}
	
	res = sourceVoice->SubmitSourceBuffer(&buffer);

	if (res) {
		FD_FATAL("Failed to submit source buffer");
	}

	memset(&emitter, 0, sizeof(X3DAUDIO_EMITTER));

	emitter.ChannelCount = 1;
	emitter.CurveDistanceScaler = 1.0f;
	emitter.DopplerScaler = 1.0f;

	AudioManager::AddAudio(this);
}

Audio::~Audio() {
	AudioManager::RemoveAudio(this);
	delete wave;
	if (sourceVoice) sourceVoice->DestroyVoice();
}

void Audio::Update() {
	X3DAUDIO_DSP_SETTINGS dsp;
	dsp.SrcChannelCount = 1;
	dsp.DstChannelCount = AudioManager::device->GetNumChannels();
	dsp.pMatrixCoefficients = new float[dsp.DstChannelCount];
	X3DAudioCalculate(AudioManager::x3dhandle, &AudioManager::listener, &emitter, X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER, &dsp);
	sourceVoice->SetOutputMatrix(AudioManager::device->GetDevice(), 1, dsp.DstChannelCount, dsp.pMatrixCoefficients);
	sourceVoice->SetFrequencyRatio(dsp.DopplerFactor);
}

void Audio::Update(vec3 position, vec3 velocity, vec3 forward, vec3 up) {
	emitter.Position = position.ToDX();
	emitter.Velocity = velocity.ToDX();
	emitter.OrientFront = forward.ToDX();
	emitter.OrientTop = up.ToDX();
	Update();
}

void Audio::UpdatePosition(vec3 position, vec3 velocity) {
	emitter.Position = position.ToDX();
	emitter.Velocity = velocity.ToDX();
	Update();
}

void Audio::UpdateOrientation(vec3 forward, vec3 up) {
	emitter.OrientFront = forward.ToDX();
	emitter.OrientTop = up.ToDX();
	Update();
}

void Audio::Play() const {
	sourceVoice->Start();
}

void Audio::Stop() const {
	sourceVoice->Stop();
}

void Audio::AddOutput(AudioMixer* mixer) {
	outputs.Push_back({ 0, mixer->GetMixer() });
	XAUDIO2_VOICE_SENDS sends;

	sends.SendCount = outputs.GetSize();
	sends.pSends = outputs.GetData();
	
	sourceVoice->SetOutputVoices(&sends);
}

void Audio::ClearOutputs() {
	outputs.Clear();
	sourceVoice->SetOutputVoices(nullptr);
}

}
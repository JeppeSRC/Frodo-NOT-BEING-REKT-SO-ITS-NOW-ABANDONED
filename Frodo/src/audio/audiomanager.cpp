#include "audiomanager.h"
#include "audio.h"
#include <core/log.h>

namespace FD {

uint32 FDGetNumChannelsFromMask(uint32 mask) {
	uint32 channels = 0;
	
	for (uint32 i = 0; i < 18; i++) {
		if (mask & (1 << i)) channels++;
	}

	return channels;
}

IXAudio2* AudioManager::engine = nullptr;
AudioDevice* AudioManager::device = nullptr;
X3DAUDIO_HANDLE AudioManager::x3dhandle;
X3DAUDIO_LISTENER AudioManager::listener;
List<Audio*> AudioManager::sources;

bool AudioManager::Init() {
	HRESULT res = XAudio2Create(&engine);

	if (res) {
		FD_FATAL("Failed to intialize XAudio");
		return false;
	}

	CreateDevice();

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

	X3DAudioInitialize(device->GetChannelMask(), 100, x3dhandle);

	return dev;
}

void AudioManager::UpdateListener(Camera* camera) {
	UpdateListener(camera->GetPosition(), camera->GetForward(), camera->GetUp(), camera->GetVelocity());
}

void AudioManager::UpdateListener(vec3 position, vec3 forward, vec3 up, vec3 velocity) {
	listener.OrientFront = forward.ToDX();
	listener.OrientTop = up.ToDX();
	listener.Position = position.ToDX();
	listener.Velocity = velocity.ToDX();
	listener.pCone = nullptr;

	uint_t size = sources.GetSize();

	for (uint_t i = 0; i < size; i++) {
		sources[i]->Update();
	}
}

void AudioManager::AddAudio(Audio* audio) {
	sources.Push_back(audio);
}

void AudioManager::RemoveAudio(Audio* audio) {
	sources.Remove(audio);
}
}

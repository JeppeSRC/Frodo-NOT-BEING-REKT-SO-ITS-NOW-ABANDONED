#pragma once

#include <fd.h>
#include <util/list.h>
#include <xaudio2.h>
#include <x3daudio.h>
#include "xadevice.h"
#include <math/vec3.h>
#include <graphics/render/camera/camera.h>

namespace FD {

uint32 FDGetNumChannelsFromMask(uint32 mask);

class FDAPI AudioManager {
private:
	friend class Audio;

	static List<Audio*> sources;
public:
	static IXAudio2* engine;
	static AudioDevice* device;
	static X3DAUDIO_HANDLE x3dhandle;
	static X3DAUDIO_LISTENER listener;

public:
	static bool Init();
	static void Release();

	static AudioDevice* CreateDevice();

	static void UpdateListener(Camera* camera);
	static void UpdateListener(vec3 position, vec3 foward, vec3 up, vec3 velocity);

	static void AddAudio(Audio* audio);
	static void RemoveAudio(Audio* audio);

};

}

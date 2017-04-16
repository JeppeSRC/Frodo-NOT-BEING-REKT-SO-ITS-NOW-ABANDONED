#pragma once
#include <math/vec2.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <fd.h>
#include <Windows.h>
#include <core/window.h>


namespace FD {

struct FD_DIRECTINPUT_DEVICE {
	GUID guidInstance;
	uint32 type;

	String instanceName;
	String productName;
};

class FDAPI Input {
private:
	static List<FD_DIRECTINPUT_DEVICE*> inputDevices;

	static IDirectInput8* dinput;
public:

	static void InitializeDirectInput();
	static void InitializeMouse(const String& name);
	static void InitializeKeyboard(const String& name);

};

}
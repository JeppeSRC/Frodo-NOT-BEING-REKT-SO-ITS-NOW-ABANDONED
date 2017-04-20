#pragma once
#include <math/vec2.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <fd.h>
#include <Windows.h>
#include "window.h"
#include <core/event/fd_key.h>

#define FD_DIRECINPUT_BUFFERSIZE 16

#define DI_FREE(x) if (x != nullptr) { \
					   x->Release();\
					   x = nullptr; }

namespace FD {

struct FD_INPUT_DEVICE {
	GUID deviceGuid;

	IDirectInputDevice8* device = nullptr;

	DIDEVCAPS cap;

	uint32 numObjects;
	DIDEVICEOBJECTINSTANCE* objects = nullptr;

	bool acquired = false;
};

struct Mouse : public FD_INPUT_DEVICE {
	ivec2 position;
};

struct Keyboard : public FD_INPUT_DEVICE {
	uint32 modifiers;

	byte k[256];
};

struct FD_DIRECTINPUT_DEVICE {
	GUID guidInstance;
	uint32 type;

	String instanceName;
	String productName;
};

class FDAPI Input {
private:
	static Window* window;

	static List<FD_DIRECTINPUT_DEVICE*> inputDevices;

	static IDirectInput8* dinput;

	static Mouse mouse;
	static Keyboard keyboard;

	static bool UpdateMouse();
	static bool UpdateKeyboard();

	static bool EnumerateObjects(FD_INPUT_DEVICE* device);

	static void HandleMouseEvents();
	static void HandleKeyboardEvents();
public:

	static String GetDeviceName(GUID guid);

	static bool InitializeDirectInput(Window* window);
	static bool InitializeMouse(const String& name = "Mouse");
	static bool InitializeMouse(GUID guid);
	static bool InitializeKeyboard(const String& name = "Keyboard");
	static bool InitializeKeyboard(GUID guid);
	
	inline static void ToggleMouseAcquisition() {
		if (mouse.acquired) {
			UnacquireMouse();
		} else {
			AcquireMouse();
		}
	}

	inline static void ToggleKeyboardAcquisition() {
		if (keyboard.acquired) {
			AcquireKeyboard();
		} else {
			UnacquireKeyboard();
		}
	}

	static void AcquireMouse();
	static void AcquireKeyboard();
	static void UnacquireMouse();
	static void UnacquireKeyboard();

	static void UpdateInputAndDispatchEvents();

	inline static bool CheckKey(FD_KEY key) { return (keyboard.k[key] & 0x80) == 1; }

	inline static ivec2 GetMousePos() { return Input::mouse.position; }
	inline static uint32 GetModifiers() { return keyboard.modifiers; }

	static void ReleaseResources();
};

}
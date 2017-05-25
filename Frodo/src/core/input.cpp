#include "input.h"
#include <memory>

namespace FD {

List<FD_DIRECTINPUT_DEVICE*> Input::inputDevices;
IDirectInput8* Input::dinput;

Mouse Input::mouse;
Keyboard Input::keyboard;

Window* Input::window;

auto cmp_name_lambda = [](FD_DIRECTINPUT_DEVICE* d, const String& name) -> bool { if (d->productName == name) return true; return false; };
auto cmp_guid_lambda = [](FD_DIRECTINPUT_DEVICE* d, GUID guid) -> bool { if (d->guidInstance == guid) return true; return false; };

String Input::GetDeviceName(GUID guid) {
	uint_t index = inputDevices.Find<GUID>(cmp_guid_lambda, guid);

	if (index == (uint_t)-1) {
		FD_WARNING("[DirectInput] GetDeviceName device doesn't exist");
		return "UNKNOW";
	}

	return inputDevices[index]->productName;
}

bool Input::InitializeDirectInput(Window* window) {
	Input::window = window;
	if (DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr) != DI_OK) {
		FD_FATAL("[DirectInput] Failed to intialize DirectInput");
		return false;
	}

	auto callback = [](LPCDIDEVICEINSTANCE dev, LPVOID param) -> BOOL {
		List<FD_DIRECTINPUT_DEVICE*>& list = *(List<FD_DIRECTINPUT_DEVICE*>*)param;

		FD_DIRECTINPUT_DEVICE* d = new FD_DIRECTINPUT_DEVICE;

		d->guidInstance = dev->guidInstance;
		d->type = dev->dwDevType;
		d->instanceName = dev->tszInstanceName;
		d->productName = dev->tszProductName;

		list.Push_back(d);

		return true;
	};

	dinput->EnumDevices(DI8DEVCLASS_ALL, callback, &inputDevices, DIEDFL_ATTACHEDONLY);

	mouse.device = nullptr;
	keyboard.device = nullptr;

	return true;
}

bool Input::UpdateMouse() {
	mouse.cap.dwSize = sizeof(DIDEVCAPS);
	HRESULT res = mouse.device->GetCapabilities(&mouse.cap);

	if (res != DI_OK) {
		FD_FATAL("[DirectInput] Failed to get mouse capabilities");
		return false;
	}

	mouse.device->SetCooperativeLevel(window->GetHWND(), DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	mouse.device->SetDataFormat(&c_dfDIMouse2);
	
	DIPROPDWORD word;
	word.diph.dwSize = sizeof(DIPROPDWORD);
	word.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	word.diph.dwObj = 0;
	word.diph.dwHow = DIPH_DEVICE;
	word.dwData = FD_DIRECINPUT_BUFFERSIZE;

	mouse.device->SetProperty(DIPROP_BUFFERSIZE, &word.diph);

	word.dwData = DIPROPAXISMODE_REL;

	mouse.device->SetProperty(DIPROP_AXISMODE, &word.diph);

	EnumerateObjects(&mouse);

	return true;
}

bool Input::UpdateKeyboard() {
	keyboard.cap.dwSize = sizeof(DIDEVCAPS);
	HRESULT res = keyboard.device->GetCapabilities(&keyboard.cap);

	if (res != DI_OK) {
		FD_FATAL("[DirectInput] Failed to get keyboard capabilities");
		return false;
	}

	keyboard.device->SetCooperativeLevel(window->GetHWND(), DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	keyboard.device->SetDataFormat(&c_dfDIKeyboard);

	DIPROPDWORD word;
	word.diph.dwSize = sizeof(DIPROPDWORD);
	word.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	word.diph.dwObj = 0;
	word.diph.dwHow = DIPH_DEVICE;
	word.dwData = FD_DIRECINPUT_BUFFERSIZE;

	keyboard.device->SetProperty(DIPROP_BUFFERSIZE, &word.diph);

	EnumerateObjects(&keyboard);

	return true;
}

bool Input::EnumerateObjects(FD_INPUT_DEVICE* device) {
	IDirectInputDevice8* d = device->device;

	auto callback = [](LPCDIDEVICEOBJECTINSTANCE obj, LPVOID param) -> BOOL { List<DIDEVICEOBJECTINSTANCE>& list = *(List<DIDEVICEOBJECTINSTANCE>*)param; list.Push_back(*obj); return true; };

	List<DIDEVICEOBJECTINSTANCE> objects;

	if (d->EnumObjects(callback, (void*)&objects, DIDFT_ALL) != DI_OK) {
		FD_WARNING("[DirectInput] Device \"%s\" object enumeration failed", *GetDeviceName(device->deviceGuid));
		return false;
	}

	delete[] device->objects;

	device->numObjects = (uint32)objects.GetSize();
	device->objects = new DIDEVICEOBJECTINSTANCE[device->numObjects];
	
	memcpy(device->objects, objects.GetData(), device->numObjects * sizeof(DIDEVICEOBJECTINSTANCE));
	return true;
}

bool Input::InitializeMouse(const String& name) {
	uint_t index = inputDevices.Find<const String&>(cmp_name_lambda, name);

	if (index == (uint_t)-1) {
		FD_WARNING("[DirectInput] Device \"%s\" doesn't exist. Using default device \"Mouse\"");
		return InitializeMouse(GUID_SysMouse);
	}

	return InitializeMouse(inputDevices[index]->guidInstance);
}

bool Input::InitializeMouse(GUID guid) {
	uint_t index = inputDevices.Find<GUID>(cmp_guid_lambda, guid);

	if (index == (uint_t)-1) {
		FD_WARNING("[DirectInput] InitializeMouse GUID not valid. Using default");
		guid = GUID_SysMouse;
	}

	String deviceName = inputDevices[index]->productName;

	HRESULT res = dinput->CreateDevice(guid, &mouse.device, NULL);

	if (res != DI_OK) {
		FD_FATAL("[DirectInput] Mouse \"%s\" initialization failed", *deviceName);
		if (guid == GUID_SysMouse) {
			FD_FATAL("[DirectInput] Default mouse failed to intialize, no mouse available");
			return false;
		}
	}

	mouse.deviceGuid = guid;

	bool result = UpdateMouse();

	if (result) FD_DEBUG("[DirecInput] Mouse \"%s\" initailized", *deviceName);
	else FD_FATAL("[DirectInput] Mouse \"%s\" initialization failed", *deviceName);

	mouse.position = ivec2(window->GetWidth() >> 1, window->GetHeight() >> 1);

	return result;
}

bool Input::InitializeKeyboard(const String& name) {
	uint_t index = inputDevices.Find<const String&>(cmp_name_lambda, name);

	if (index == (uint_t)-1) {
		FD_WARNING("[DirectInput] Device \"%s\" doesn't exist. Using default device \"Keyboard\"");
		return InitializeKeyboard(GUID_SysKeyboard);
	}

	return InitializeKeyboard(inputDevices[index]->guidInstance);
}

bool Input::InitializeKeyboard(GUID guid) {
	uint_t index = inputDevices.Find<GUID>(cmp_guid_lambda, guid);

	if (index == (uint_t)-1) {
		FD_WARNING("[DirectInput] InitializeKeyboard GUID not valid. Using default");
		guid = GUID_SysKeyboard;
	}

	String deviceName = inputDevices[index]->productName;

	HRESULT res = dinput->CreateDevice(guid, &keyboard.device, NULL);

	if (res != DI_OK) {
		FD_FATAL("[DirectInput] Mouse \"%s\" initialization failed", *deviceName);
		if (guid == GUID_SysKeyboard) {
			FD_FATAL("[DirectInput] Default mouse failed to intialize, no mouse available");
			return false;
		}
	}

	keyboard.deviceGuid = guid;

	bool result = UpdateKeyboard();

	if (result) FD_DEBUG("[DirecInput] Keyboard \"%s\" initailized", *deviceName);
	else FD_DEBUG("[DirecInput] Keyboard \"%s\" initailization failed", *deviceName);

	return result;
}

void Input::ReleaseResources() {
	UnacquireKeyboard();
	UnacquireMouse();
	
	DI_FREE(mouse.device);
	DI_FREE(keyboard.device);
	DI_FREE(dinput);
}

void Input::AcquireMouse() {
	if (mouse.device) {
		mouse.device->Acquire();
		mouse.acquired = true;
	}
}

void Input::AcquireKeyboard() {
	if (keyboard.device) {
		keyboard.device->Acquire();
		keyboard.acquired = true;
	}
}

void Input::UnacquireMouse() {
	if (mouse.device) {
		mouse.device->Unacquire();
		mouse.acquired = false;
	}
}

void Input::UnacquireKeyboard() {
	if (keyboard.device) {
		keyboard.device->Unacquire();
		keyboard.acquired = false;
	}
}


}
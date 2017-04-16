#include "input.h"
#include <memory>

namespace FD {

List<FD_DIRECTINPUT_DEVICE*> Input::inputDevices;
IDirectInput8* Input::dinput;

void Input::InitializeDirectInput() {
	if (DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr) != DI_OK) {
		FD_FATAL("[DirectInput] Failed to intialize DirectInput");
		return;
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

	dinput->EnumDevices(DI8DEVCLASS_ALL, callback, &inputDevices, DIEDFL_ALLDEVICES);



}

}
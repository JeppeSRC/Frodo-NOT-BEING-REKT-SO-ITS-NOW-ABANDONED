#include "d3doutput.h"
#include <core/log.h>

namespace FD {

D3DOutput::D3DOutput(IDXGIOutput* output) {
	FD_ASSERT(output == nullptr);

	DXGI_OUTPUT_DESC desc;

	output->GetDesc(&desc);

	this->output = output;
	
	name = String(desc.DeviceName);

	desktopCoordinates = desc.DesktopCoordinates;
	attachedToDesktop = desc.AttachedToDesktop;
	rotation = (FD_MONITOR_ROTATION)desc.Rotation;
	monitorHandle = desc.Monitor;

	uint32 numModes = 0;

	output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, nullptr);

	if (!numModes) return;

	DXGI_MODE_DESC* descs = new DXGI_MODE_DESC[numModes];

	output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, descs);

	for (uint_t i = 0; i < numModes; i++) {
		modes.Push_back(descs[i]);
	}
}

D3DOutput::~D3DOutput() {
	output->Release();
	output = nullptr;
}

}
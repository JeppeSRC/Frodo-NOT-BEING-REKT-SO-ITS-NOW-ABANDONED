#pragma once

#include <fd.h>
#include <d3d11.h>
#include <d3d10.h>
#include <dxgi.h>
#include <util/list.h>
#include <util/string.h>

namespace FD {

enum FD_MONITOR_ROTATION {
	FD_MONITOR_ROTATION_UNSPECIFIED,
	FD_MONITOR_ROTATION_IDENTITY,
	FD_MONITOR_ROTATION_ROTATE90 ,
	FD_MONITOR_ROTATION_ROTATE180,
	FD_MONITOR_ROTATION_ROTATE270
};

class FDAPI D3DOutput {
private:
	IDXGIOutput* output;

	String name;

	RECT desktopCoordinates;
	bool attachedToDesktop;
	FD_MONITOR_ROTATION rotation;
	HMONITOR monitorHandle;

	List<DXGI_MODE_DESC> modes;

	DXGI_MODE_DESC currentMode;

public:
	D3DOutput(IDXGIOutput* output);
	~D3DOutput();

	inline const String& GetName() const { return name; }

	inline RECT GetDesktopCoordinates() const { return desktopCoordinates; }
	inline bool IsAttachedToDesktop() const { return attachedToDesktop; }
	inline FD_MONITOR_ROTATION GetRotation() const { return rotation; }
	inline HMONITOR GetMonitorHandle() const { return monitorHandle; }

	inline const List<DXGI_MODE_DESC>& GetModes() const { return modes; }
	inline DXGI_MODE_DESC GetBestMode() const { return modes.Get(modes.GetSize() - 1); }

	inline DXGI_MODE_DESC GetCurrentMode() const { return currentMode; }
	inline void SetCurrentMode(DXGI_MODE_DESC desc) { this->currentMode = desc; }

	inline IDXGIOutput* GetOutput() const { return output; }

};

}
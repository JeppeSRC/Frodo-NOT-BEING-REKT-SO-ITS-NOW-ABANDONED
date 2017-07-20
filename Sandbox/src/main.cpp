#include <stdio.h>
#include "test.h"
#include "pbrtest.h"
#include "rTest.h"
#include <core/log.h>
#include <util/wave.h>
#include <string>
#include <util/vfs/vfs.h>

#define PRINT(str) FD_INFO("%s", *str)

using namespace FD;

int main() {
	/*
	D3DFactory::CreateFactory();
	
	List<D3DAdapter*> adapters = D3DFactory::GetAdapters();

	List<D3DOutput*> outputs = adapters[0]->GetOutputs();

	D3DOutput* o = outputs.Get(1);

	List<DXGI_MODE_DESC> modes = o->GetModes();

	FD_INFO("Modes:");
	for (uint_t i = 0; i < modes.GetSize(); i++) {
		DXGI_MODE_DESC m = modes.Get(i);
		FD_INFO("Mode: %u", i);
		FD_INFO("\tWidth: %u", m.Width);
		FD_INFO("\tHeight: %u", m.Height);
		FD_INFO("\tRefresh Rate: %f", (float32)m.RefreshRate.Numerator / m.RefreshRate.Denominator);
		FD_INFO("\tScan Order: %s", m.ScanlineOrdering == 0 ? "UNSPECIFIED" : m.ScanlineOrdering == 1 ? "PROGRESSIVE" : m.ScanlineOrdering == 2 ? "UPPER FIRST" : "LOWER FIRST");
		FD_INFO("\tScaling: %s", m.Scaling == 0 ? "UNSPECIFIED" : m.Scaling == 1 ? "CENTER" : "STRETCHED");
	}

	return 0;*/

	RTest test;
	test.Start();

	return 0;
}
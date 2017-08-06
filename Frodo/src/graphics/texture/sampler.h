#pragma once

#include <fd.h>
#include <graphics/d3dcontext.h>

namespace FD {

class FDAPI Sampler {
protected:
	ID3D11SamplerState* samplerState;

public:
	static D3D11_SAMPLER_DESC GetDefaultDesc();

public:
	Sampler();
	Sampler(D3D11_SAMPLER_DESC sd);
	~Sampler();

	void Bind(uint32 slot = 0) const;

};

}
#include "sampler.h"
#include <core/log.h>

namespace FD {

D3D11_SAMPLER_DESC Sampler::GetDefaultDesc() {
	D3D11_SAMPLER_DESC sd;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.MinLOD = -FLT_MAX;
	sd.MaxLOD = FLT_MAX;
	sd.MipLODBias = 0;
	sd.MaxAnisotropy = 1;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.BorderColor[0] = 1.0f;
	sd.BorderColor[1] = 1.0f;
	sd.BorderColor[2] = 1.0f;
	sd.BorderColor[3] = 1.0f;

	return sd;
}

Sampler::Sampler() : Sampler(GetDefaultDesc()) { }

Sampler::Sampler(D3D11_SAMPLER_DESC sd) {
	samplerState = nullptr;

	D3DContext::GetDevice()->CreateSamplerState(&sd, &samplerState);

	FD_ASSERT(samplerState == nullptr);
}

Sampler::~Sampler() {
	DX_FREE(samplerState);
}

void Sampler::Bind(uint32 slot) const {
	D3DContext::GetDeviceContext()->PSSetSamplers(slot, 1, &samplerState);
}

}
R"(
SamplerState samp {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

Texture2D tex : register(t0);

float4 psMain(float4 position : SV_POSITION, float2 texCoord : TEXCOORD) : SV_TARGET {
	float3 r = tex.Sample(samp, texCoord).rgb;

	return float4(r, 1);
}

)"
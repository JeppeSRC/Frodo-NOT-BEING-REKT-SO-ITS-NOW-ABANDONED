R"(SamplerState state {
	Filter = MIN_MAX_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct MRTOut {
	float3 position : SV_TARGET0;
	float3 diffuse : SV_TARGET1;
	float3 normal : SV_TARGET2;
};

Texture2D diffuseTexture : register(t0);

MRTOut psMain(float4 position : SV_POSITION, float3 pos : POSITION, float2 texCoord : TEXCOORD, float3 normal : NORMAL) {
	MRTOut o;

	o.position = pos;
	o.diffuse = float3(1, 0, 1);
	o.normal = normal;

	return o;
}
)"
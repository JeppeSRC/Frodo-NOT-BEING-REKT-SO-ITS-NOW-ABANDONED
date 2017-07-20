R"(
SamplerState state {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct Light {
	float3 color;
	float pad0;
	float3 direction;
	float pad1;	
};

cbuffer Light : register(b0) {
	Light light;
};

Texture2D positions : register(t0);
Texture2D diffuse : register(t1);
Texture2D normalss : register(t2);

float4 psMain(float4 position : SV_POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD) : SV_TRAGET0 {
	float3 pos = positions.Sample(state, texCoord).xyz;
	float3 col = diffuse.Sample(state, texCoord).xyz;
	float3 norm = normals.Sample(state, texCoord).xyz;

	float brightness = max(dot(norm, -light.direction), 0.03);
}

)"
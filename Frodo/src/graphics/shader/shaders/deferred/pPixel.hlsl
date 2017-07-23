R"(
SamplerState state {
	Filter = MIN_MAX_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct Light {
	float3 color;
	float pad1;
	float3 position;
	float pad3;
	float3 attenuation;
	float pad4;
};

cbuffer Light : register(b0) {
	Light light;
};

Texture2D positions : register(t0);
Texture2D diffuse : register(t1);
Texture2D normals : register(t2);

float4 psMain(float4 position : SV_POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD) : SV_TARGET0 {

	float3 pos = positions.Sample(state, texCoord).xyz;
	float3 col = diffuse.Sample(state, texCoord).xyz;
	float3 norm = normals.Sample(state, texCoord).xyz;

	float3 dir = light.position - pos;
	float dist = length(dir);
	dir = normalize(dir);
	float attenuation = 1.0f / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * dist * dist + 0.000001);
	float brightness = max(dot(norm, dir), 0.03) * attenuation;

	return float4(col * brightness, 1);
}
)"
R"(

SamplerState samp {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct Light {
	float3 color;
	float pad0;
	float3 position;
	float pad1;
	float3 attenuation;
	float pad2;
};

struct Material {
	float3 color;
	float reflectiveness;
};

cbuffer Light : register(b0) {
	Light light;
};

cbuffer Material : register(b1) {
	Material material;
};

Texture2D diffuse : register(t0);
Texture2D shadowMap : register(t1);

float4 psMain(float4 position : SV_POSITION, float3 pos : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD, float4 posLightSpace : LIGHTPOS) : SV_TARGET0 {

	float3 dir = light.position - pos;
	float dist = length(dir);
	dir = normalize(dir);

	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * dist * dist + 0.0000001);
	float brightness = dot(normal, dir) * attenuation;

	float3 finalColor = diffuse.Sample(samp, texCoord).xyz;

	return float4(finalColor * material.color * light.color * brightness, 1);
}

)"
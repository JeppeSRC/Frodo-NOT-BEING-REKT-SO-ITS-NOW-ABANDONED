R"(

SamplerState samp {
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

float SampleShadowMap(float4 vertPos) {
	float3 coord = vertPos.xyz;

	coord.xy = coord.xy * -0.5 + 0.5;

	float depth = shadowMap.Sample(samp, coord.xy).r;

	return depth + 0.01 > coord.z ? 1 : 0;
}

float4 psMain(float4 position : SV_POSITION, float3 pos : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD, float4 posLightSpace : LIGHTPOS) : SV_TARGET0 {

	float brightness = dot(normal, -light.direction);

	float3 finalColor = diffuse.Sample(samp, texCoord).xyz;

	float3 d = posLightSpace.xyz/posLightSpace.w;

	d = d * 0.5 + 0.5;

	//if (d.x < 0.0)
	//	return float4(d.x, d.y, 0, 1);

	float depth = SampleShadowMap(posLightSpace);


	return float4(finalColor * material.color * light.color * brightness * depth, 1);
}


)"
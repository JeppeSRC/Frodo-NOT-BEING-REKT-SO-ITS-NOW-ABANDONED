R"(

SamplerState samp : register(s0);
SamplerState shadowSamp : register(s1);

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

float SampleShadowMap(float4 vertPos, float bias) {
	float3 coord = vertPos.xyz;

	if (coord.z > 20) return 1.0;

	coord.xy = coord.xy * -0.5 + 0.5;

	float depth = 0;//shadowMap.Sample(shadowSamp, float2(1 - coord.x, coord.y)).r;

	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			depth += shadowMap.Sample(shadowSamp, float2(1 - (coord.x + x * 0.00048828125), coord.y + y * 0.00048828125)).r;
		}
	}

	depth /= 9;

	return depth + bias > coord.z ? 1 : 0;
}

float4 psMain(float4 position : SV_POSITION, float3 pos : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD, float4 posLightSpace : LIGHTPOS) : SV_TARGET0 {

	float brightness = dot(normal, -light.direction);

	float3 finalColor = diffuse.Sample(samp, texCoord).xyz;

#shaderGen if defined(SHADOW)
	float depth = SampleShadowMap(posLightSpace, max(0.01 * (1 - brightness), 0.001));

//	return float4(depth, depth, depth, 1);

	return float4(finalColor * material.color * light.color * brightness * depth, 1);

#shaderGen else
	return float4(finalColor * material.color * light.color * brightness, 1);
#shaderGen endif
}


)"
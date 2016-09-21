R"( 


SamplerState samp{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Warp;
	AddressV = Warp;
};

cbuffer lightData : register(b0) {
	float3 l_color;
	float pad0;
	float3 l_position;
	float pad1;
	float3 l_attenuation;
	float pad2;
};

cbuffer materialData : register(b1) {
	float4 m_color;
};

Texture2D diffuseTexure : register(t0);

float4 psMain(float4 pos : SV_POSITION, float3 position : POSITION, float2 texCoord : TEXCOORDS, float3 normal : NORMALS) : SV_TARGET {

	float3 dirToLight = l_position - position;
	float distToLight = length(dirToLight);

	dirToLight = normalize(dirToLight);

	float attenuation = 1.0f / (l_attenuation.x + l_attenuation.y * distToLight + l_attenuation.z * distToLight + distToLight);

	float brightness = dot(normal, dirToLight) * attenuation;

	float3 diffuse = diffuseTexure.Sample(samp, texCoord).xyz * l_color * m_color.xyz;

	return float4(diffuse * brightness, 1);
}

)"

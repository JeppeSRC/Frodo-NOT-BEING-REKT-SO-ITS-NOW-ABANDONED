R"(

SamplerState samp {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

cbuffer lightData : register(b0) {
	float3 l_color;
	float pad0;
	float3 l_position;
	float pad1;
	float3 l_attenuation;
	float pad2;
	float3 l_direction;
	float pad3;
	float2 l_cutoffExponent;
	float2 pad4;
};

cbuffer materialData : register(b1) {
	float4 m_color;
};

Texture2D diffuseTexture : register(t0);

float4 psMain(float4 pos : SV_POSITION, float3 position : POSITION, float2 texCoord : TEXCOORDS, float3 normal : NORMALS) : SV_TARGET {

	float3 dirToLight = l_position - position;
	float distToLight = length(dirToLight);

	dirToLight = normalize(dirToLight);

	float NdotL = dot(normal, dirToLight);
	float attenuation = 0;

	if (NdotL > 0.0) {
		float spotEffect = dot(-dirToLight, l_direction);
		if (spotEffect > l_cutoffExponent.x) {
			spotEffect = pow(abs(spotEffect), l_cutoffExponent.y);
			attenuation = spotEffect / (l_attenuation.x + l_attenuation.y * distToLight + l_attenuation.z * distToLight * distToLight);
		}
	}

	float3 diffuse = m_color.xyz * l_color * diffuseTexture.Sample(samp, texCoord).xyz;

	return float4(diffuse * attenuation * NdotL, 1);
}

)"

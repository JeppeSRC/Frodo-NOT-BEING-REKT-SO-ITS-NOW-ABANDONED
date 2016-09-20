R"(

SamplerState samp {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

Texture2D texPosition : register(t0);
Texture2D texDiffuse : register(t1);
Texture2D texNormals : register(t2);
//Texture2D shadowMap : register(t3);


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

float4 psMain(float4 pos : SV_POSITION, float2 texCoords : TEXCOORDS) : SV_TARGET {
	
	float3 position = texPosition.Sample(samp, texCoords).xyz;
	float3 diffuse = texDiffuse.Sample(samp, texCoords).xyz;
	float3 normal = texNormals.Sample(samp, texCoords).xyz;

	float3 dirToLight = l_position - position;
	float distToLight = length(dirToLight);
	
	dirToLight = normalize(dirToLight);

	float NdotL = dot(normal, dirToLight);
	float attenuation = 0;

	if (NdotL > 0.0) {
		float spotEffect = dot(l_direction, -dirToLight);
		if (spotEffect > l_cutoffExponent.x) {
			spotEffect = pow(abs(spotEffect), l_cutoffExponent.y);
			attenuation = spotEffect / (l_attenuation.x + l_attenuation.y * distToLight + l_attenuation.z * distToLight * distToLight);
		}
	}
	
	return float4(l_color * diffuse * attenuation, 1);
}

)"
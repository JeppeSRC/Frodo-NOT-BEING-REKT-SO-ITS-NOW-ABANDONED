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
	float3 l_position;
	float3 l_attenuation;
};

float4 psMain(float4 pos : SV_POSITION, float2 texCoords : TEXCOORDS) : SV_TARGET {
	
	float3 position = texPosition.Sample(samp, texCoords).xyz;
	float3 diffuse = texDiffuse.Sample(samp, texCoords).xyz;
	float3 normal = texNormals.Sample(samp, texCoords).xyz;

	float3 dir = l_position - position;
	float dist = length(dir);
	
	dir = normalize(dir);

	float attenuation = 1.0f / (l_attenuation.x + l_attenuation.y * dist + l_attenuation.z * dist * dist + 0.000001f);

	float d = dot(dir, normal);
	
	float brightness  = d * attenuation;

	return float4(l_color * diffuse * brightness, 1);
}

)"
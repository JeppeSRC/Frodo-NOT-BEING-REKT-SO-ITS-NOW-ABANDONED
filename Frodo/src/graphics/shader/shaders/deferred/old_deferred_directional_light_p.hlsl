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
	float pad;
	float3 l_direction;
	float pad1;
};

float4 psMain(float4 pos : SV_POSITION, float2 texCoords : TEXCOORDS) : SV_TARGET {
	
	float3 diffuse = texDiffuse.Sample(samp, texCoords).xyz;
	float3 normal = texNormals.Sample(samp, texCoords).xyz;

	return float4((l_color * diffuse) * dot(-l_direction, normal), 1);
}

)"
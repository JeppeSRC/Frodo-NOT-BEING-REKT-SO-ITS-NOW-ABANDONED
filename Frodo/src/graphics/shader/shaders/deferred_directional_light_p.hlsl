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


float4 psMain(float4 pos : SV_POSITION, float2 texCoords : TEXCOORDS) : SV_TARGET {
	
	float3 diffuse = texDiffuse.Sample(samp, texCoords).xyz;
	float3 normal = texNormals.Sample(samp, texCoords).xyz;

	
	return float4(diffuse, 1);
}

)"
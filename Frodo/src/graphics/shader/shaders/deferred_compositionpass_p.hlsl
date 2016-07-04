R"(

SamplerState samp {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

Texture2D position : register(t0);
Texture2D diffuse : register(t1);
Texture2D normals : register(t2);
//Texture2D shadowMap : register(t3);


float4 psMain(float4 pos : SV_POSITION, float2 texCoords : TEXCOORDS) : SV_TARGET {
	
	return diffuse.Sample(samp, texCoords);
}

)"
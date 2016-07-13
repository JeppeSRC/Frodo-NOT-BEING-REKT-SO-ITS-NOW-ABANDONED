R"(

SamplerState samp {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct MRTOut {
	float3 position : SV_TARGET0;
	float3 diffuse : SV_TARGET1;
	float3 normals : SV_TARGET2;
	//float3 shadwowMap : SV_TARGET3; 

};

Texture2D diffuseTexture : register(t0);

cbuffer materialData : register(b0) {
	float4 color;
};


MRTOut psMain(float4 position : SV_POSITION, float3 pos : POSITION, float2 texCoord : TEXCOORDS, float3 normal : NORMALS) {
	
	MRTOut o;

	o.position = pos;
	o.diffuse = (color * diffuseTexture.Sample(samp, texCoord)).xyz;
	o.normals = normal;

	return o;
}
)"
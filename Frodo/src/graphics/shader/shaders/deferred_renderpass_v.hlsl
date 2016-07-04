R"(

struct Out {
	float4 position : SV_POSITION;
	float3 pos : POSITION;
	float2 texCoord : TEXCOORDS;
	float3 normal : NORMALS;
};

cbuffer proj : register(b0) {
	float4x4 projection; 
};

cbuffer modelData : register(b1) {
	float4x4 translation;
	float4x4 rotation;
};

Out vsMain(float3 position : POSITION, float2 texCoords : TEXCOORDS, float3 normals : NORMALS) {
	Out o;

	o.position =  mul(projection, mul(translation, mul(rotation, float4(position, 1))));
	o.pos = position;
	o.texCoord = texCoords;
	o.normal = mul(float4(normals, 0), rotation).xyz;

	return o;
}


)"
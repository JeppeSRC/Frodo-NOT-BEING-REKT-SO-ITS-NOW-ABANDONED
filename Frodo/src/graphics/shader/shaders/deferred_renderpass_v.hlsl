R"(
struct Out {
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORDS;
	float3 normal : NORMALS;
};

cbuffer proj : register(b0) { float4x4 projection; }

cbuffer modelData : register(b1) {
	float4x4 translation;
	float4x4 rotation;
	float4 color;
};

Out vsMain(float3 position : POSITION, float2 texCoords : TEXCOORDS, float3 normals : NORMALS) {
	Out o;

	o.position = mul(projection, mul(translation, mul(rotation, float4(position, 1))));
	o.color = color;
	o.texCoord = texCoords;
	o.normal = normals;

	return o;
}


)"
R"(


struct Out {
	float4 pos : SV_POSITION;
	float3 position : POSITION;
	float2 texCoord : TEXCOORDS;
	float3 normal : NORMALS;
};

cbuffer viewData : register(b0) {
	float4x4 projection;
	float4x4 view;
};

cbuffer modelData : register(b1) {
	float4x4 translation;
	float4x4 rotation;
	float4x4 scale;
};

Out vsMain(float3 position : POSITION, float2 texCoords : TEXCOORDS, float3 normal : NORMALS) {

	Out o;

	float4 translatedPos = mul(translation, mul(rotation, mul(scale, float4(position, 1))));

	o.pos = mul(projection, mul(view, translatedPos));
	o.position = translatedPos.xyz;
	o.texCoord = texCoords;
	o.normal = mul(rotation, float4(normal, 0)).xyz;

	return o;
}

)"
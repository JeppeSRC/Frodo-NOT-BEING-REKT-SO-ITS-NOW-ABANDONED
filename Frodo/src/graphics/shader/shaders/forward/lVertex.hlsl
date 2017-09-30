R"(

struct Out {
	float4 position : SV_POSITION;
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 posLightSpace : LIGHTPOS;
};

cbuffer Camera : register(b0) {
	float3 c_Position;
	float c_Pad0;
	float4x4 c_ViewMatrix;
	float4x4 c_ProjectionMatrix;
};

cbuffer Model : register(b1) {
	float4x4 m_ModelMatrix;
};

cbuffer LightMatrix : register(b2) {
	float4x4 l_LightMatrix;
};

Out vsMain(float3 positions : POSITION, float3 normals : NORMAL, float2 texCoords : TEXCOORD) {
	Out o;

	float4 pos = mul(m_ModelMatrix, float4(positions, 1));

	o.position = mul(c_ProjectionMatrix, mul(c_ViewMatrix, pos));
	o.pos = pos.xyz;
	o.normal = normalize(mul(m_ModelMatrix, float4(normals, 0)).xyz);
	o.texCoord = texCoords;
	o.posLightSpace = mul(l_LightMatrix, pos);

	return o;
}

)"
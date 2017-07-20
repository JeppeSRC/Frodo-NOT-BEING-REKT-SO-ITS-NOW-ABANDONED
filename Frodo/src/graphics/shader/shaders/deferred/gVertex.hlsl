R"(
struct Out {
	float4 position : SV_POSITION;
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
};

cbuffer Camera : register(b0) {
	float3 c_Position;
	float c_Pas0;
	float4x4 c_ViewMatrix;
	float4x4 c_ProjectionMatrix;
};

cbuffer Model : register(b1) {
	float4x4 m_ModelMatrix;
};

Out vsMain(float3 position : POSITION, float3 normals : NORMAL, float2 texCoords : TEXCOORD) {
	
	Out o;

	float4 vertPos = mul(m_ModelMatrix, float4(position, 1));
	
	o.position = mul(c_ProjectionMatrix, mul(c_ViewMatrix, vertPos));
	o.pos = vertPos.xyz;
	o.texCoord = texCoords;
	o.normal = normalize(mul(m_ModelMatrix, float4(normals, 0))).xyz;

	return o;
}
)"
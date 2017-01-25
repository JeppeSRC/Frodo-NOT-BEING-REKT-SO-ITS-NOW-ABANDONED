
struct Out {
	float4 pos : SV_POSITION;
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float3 camPos : CAMPOS;
};

cbuffer Camera : register(b1) {
	float3 c_Position;
	float c_Pad0;
	float4x4 c_ViewMatrix;
	float4x4 c_ProjectionMatrix;
};

cbuffer Model : register(b5) {
	float4x4 m_ModelMatrix;
};

Out vsMain(float3 position : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD) {
	Out o;
	o.pos = mul(c_ProjectionMatrix, mul(c_ViewMatrix, mul(m_ModelMatrix, float4(position, 1))));
	o.position = mul(m_ModelMatrix, float4(position, 1)).xyz;
	o.normal = mul(m_ModelMatrix, float4(normal, 0)).xyz;
	o.texCoord = texCoord;
	o.camPos = c_Position;

	return o;
}

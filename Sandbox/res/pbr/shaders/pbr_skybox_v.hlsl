

struct Out {
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

cbuffer Camera : register(b1) {
	float3 c_Position;
	float c_Pad0;
	float4x4 c_ViewMatrix;
	float4x4 c_ProjectionMatrix;
};

cbuffer Model : register(b2) {
	float4x4 m_ModelMatrix;
};

cbuffer InverseViewMatrix : register(b3) {
	float4x4 m_InverseView;
};

Out vsMain(float3 position : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD) {
	Out o;
	float4 pos = float4(position, 1);
	pos.z = 0.99999;
	o.position = pos;
	pos.w = 0;
	o.texCoord = mul(m_InverseView, pos).xyz;

	return o;
}
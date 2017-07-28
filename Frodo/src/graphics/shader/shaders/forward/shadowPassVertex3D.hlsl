R"(

cbuffer Model : register(b0) {
	float4x4 m_Model;
};

float4 vsMain(float3 position : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD) : SV_POSITION {
	return mul(m_Model, float4(position, 1));
}

)"
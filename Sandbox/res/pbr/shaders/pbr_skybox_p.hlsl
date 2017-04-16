

cbuffer Light : register(b0) {
	float3 l_Direction;
	float l_pad2;
	float3 l_Color;
	float l_pad0;
	float3 l_Position;
	float l_pad1;
	float3 l_Attenuation;
	float pad2;
};
SamplerState samp {
	
	Filter = MIN_MAG_LINEAR;
};

TextureCube m_EnvironmentMap : register(t0);

float4 psMain(float4 position : SV_POSITION, float3 texCoord : TEXCOORD) : SV_TARGET0 {
	return m_EnvironmentMap.Sample(samp, texCoord);
}
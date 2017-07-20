
SamplerState samp {
	
	Filter = MIN_MAG_LINEAR;
};

TextureCube m_EnvironmentMap : register(t0);

float4 psMain(float4 position : SV_POSITION, float3 texCoord : TEXCOORD) : SV_TARGET0 {
	return m_EnvironmentMap.Sample(samp, texCoord);
}
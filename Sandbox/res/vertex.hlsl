

float4 vsMain(float3 position : POSITION) : SV_POSITION {
	return float4(position, 1);
}




float4 psMain(float4 position : SV_POSITION, float2 texCoords : TEXCOORDS, float tid : TID, float4 color : COLOR) : SV_TARGET{
	return color;
}
R"(

float4 psMain(float4 position : SV_POSITION, float4 color : COLOR, float2 texCoord : TEXCOORDS, float3 normal : NORMALS) : SV_TARGET {
	
	return color;

}
)"
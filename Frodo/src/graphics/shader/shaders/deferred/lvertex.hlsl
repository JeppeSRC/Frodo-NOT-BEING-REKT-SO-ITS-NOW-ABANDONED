R"(
struct Out {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

Out vsMain(float3 position : POSITION, float3 normals : NORMAL, float2 texCoords : TEXCOORD) {
	Out o;

	o.position = float4(position, 1);
	o.texCoord = texCoords;
	o.normal = normals;

	return o;
}
)"
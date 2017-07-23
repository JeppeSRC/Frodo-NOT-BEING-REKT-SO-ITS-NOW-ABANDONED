R"(

struct Out {
	float4 position : SV_POSITION;
	float depth : DEPTH;
};

cbuffer MVP : register(b0) {
	float4x4 l_LightMatrix;
};

Out vsMain(float3 position : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD) {
	Out o;

	float4 pos = mul(l_LightMatrix, float4(position, 1));
	o.position = pos;
	o.depth = pos.z/pos.w;

	return o;
}

)"
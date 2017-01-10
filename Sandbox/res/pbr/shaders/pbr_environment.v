

struct Out {
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

struct In {
	float3 position : POSITION;
	float3 texCoord : TEXCOORD;
};

cbuffer data : register(b0) {
	float4x4 invViewProjection;
};

Out vsMain(In i) {
	Out o;

	o.position = float4(i.position, 1);
	o.texCoord = mul(invViewProjection, float4(i.position, 0)).xyz;

	return o;
}
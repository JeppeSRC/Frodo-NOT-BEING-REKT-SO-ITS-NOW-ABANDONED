
struct Out {
	float4 position : SV_POSITION;
	float2 texCoords : TEXCOORDS;
};

cbuffer buffer : register(b0) {
	float4x4 cb_projection;
	float4x4 cb_model;
};

Out vsMain(float4 position : POSITION, float2 texCoords : TEXCOORDS) {
	Out o;

	o.position = mul(cb_projection, mul(cb_model, position));
	o.texCoords = texCoords;

	return o;
}
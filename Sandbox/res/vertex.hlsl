
struct Out {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer buffer : register(b0) {
	float4x4 cb_projection;
	float4x4 cb_model;
};

Out vsMain(float4 position : POSITION, float4 color : COLOR) {
	Out o;

	o.position = mul(cb_projection, mul(cb_model, position));
	o.color = color;

	return o;
}
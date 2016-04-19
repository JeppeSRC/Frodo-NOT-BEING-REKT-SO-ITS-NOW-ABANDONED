
struct Out {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer buffer : register(b0) {
	float4x4 translation;
	float4x4 rotation;
};

Out vsMain(float4 position : POSITION, float4 color : COLOR) {
	Out o;

	o.position = mul(translation, mul(rotation, position));
	o.color = color;

	return o;
}
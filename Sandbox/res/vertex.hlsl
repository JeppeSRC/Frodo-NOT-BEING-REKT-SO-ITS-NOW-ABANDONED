
struct Out {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer buffer : register(b12) {
	float4 model;
	float4 view;
	float4 projection;
};


Out vsMain(float4 position : POSITION, float4 color : COLOR) {
	Out o;

	o.position = position;
	o.color = color;

	return o;
}
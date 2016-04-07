
struct Out {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

Out vsMain(float4 position : POSITION, float4 color : COLOR) {
	Out o;

	o.position = position;
	o.color = color;

	return o;
}
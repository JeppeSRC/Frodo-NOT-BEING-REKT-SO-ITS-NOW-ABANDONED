R"(

struct GSOut {
	float4 position : SV_Position;
	float3 pos : POSITION;
	uint index : SV_RenderTargetArrayIndex;
};

cbuffer LightMatrix : register(b0) {
	float4x4 l_LightMatrix[6];
};

[maxvertexcount(18)]
void gsMain(triangle float4 position[3] : SV_Position, inout TriangleStream<GSOut> output) {

	GSOut o;

	for (uint index = 0; index < 6; index++) {
		if (!(index == 3)) continue;
		for (uint i = 0; i < 3; i++) {
			o.position = mul(l_LightMatrix[index], position[i]);
			o.pos = o.position.xyz;
			o.index = index;

			output.Append(o);
		}
		output.RestartStrip();
	}
}

)"
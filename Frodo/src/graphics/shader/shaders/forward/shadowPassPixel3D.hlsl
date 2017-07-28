R"(

cbuffer ProjectionDepth : register(b0) {
	float maxDepth;
	float3 pad0;
};

cbuffer LightPosition : register(b1) {
	float3 l_Position;
	float pad1;
};

float psMain(float4 position : SV_POSITION, float3 pos : POSITION, uint index : SV_RenderTargetArrayIndex) : SV_Target {
	float dist = length(l_Position - pos);

	return dist;
}

)"
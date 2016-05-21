

struct Out {
	float4 position : SV_POSITION;
	float2 texCoords : TEXCOORDS;
	float tid : TID;
	float4 color : COLOR;
};

cbuffer projection_buffer : register(b0) {
	float4x4 projection;
};

Out vsMain(float2 position : POSITION, float2 texCoords : TEXCOORDS, float tid : TID, float4 color : COLOR) {
	Out o;

	o.position = mul(projection, float4(position, 0, 1));
	o.texCoords = texCoords;
	o.tid = tid;
	o.color = color;

	return o;
}
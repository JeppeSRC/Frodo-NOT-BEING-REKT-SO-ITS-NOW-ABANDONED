
struct In {
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

TextureCube cubeTexture : register(t0);

SamplerState state {
	AdressU = Wrap;
	AdressV = Wrap;
	AdressW = Wrap;
	Filter = MIN_MAG_MIP_LINEAR;
};

float4 psMain(In i) : SV_TARGET {

	
	return cubeTexture.Sample(state, i.texCoord);
}
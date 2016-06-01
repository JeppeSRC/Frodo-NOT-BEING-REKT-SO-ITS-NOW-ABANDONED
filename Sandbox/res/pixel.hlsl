Texture2D tex : register(t0);

SamplerState sampState {
	AddressU = Wrap;
	AddressV = Wrap;
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = NONE;
};

float4 psMain(float4 position : SV_POSITION, float2 texCoords : TEXCOORDS) : SV_TARGET{

	return tex.Sample(sampState, texCoords);
}


Texture2D textures[32] : register(t0);

SamplerState sampState {
	AddressU = Wrap;
	AddressV = Wrap;
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = NONE;
};

float4 psMain(float4 position : SV_POSITION, float2 texCoords : TEXCOORDS, float tid : TID, float4 color : COLOR) : SV_TARGET {
	
	float4 texColor = color;
	if (tid > 0.0){
		switch (tid) {
			case 1.0:
				texColor *= textures[0].Sample(sampState, texCoords);
				break;
			case 2.0:
				texColor *= textures[1].Sample(sampState, texCoords);
				break;
			case 3.0:
				texColor *= textures[2].Sample(sampState, texCoords);
				break;
			case 4.0:
				texColor *= textures[3].Sample(sampState, texCoords);
				break;
			case 5.0:
				texColor *= textures[4].Sample(sampState, texCoords);
				break;
			case 6.0:
				texColor *= textures[5].Sample(sampState, texCoords);
				break;
			case 7.0:
				texColor *= textures[6].Sample(sampState, texCoords);
				break;
			case 8.0:
				texColor *= textures[7].Sample(sampState, texCoords);
				break;
			case 9.0:
				texColor *= textures[8].Sample(sampState, texCoords);
				break;
			case 10.0:
				texColor *= textures[9].Sample(sampState, texCoords);
				break;
			case 11.0:
				texColor *= textures[10].Sample(sampState, texCoords);
				break;
			case 12.0:
				texColor *= textures[11].Sample(sampState, texCoords);
				break;
			case 13.0:
				texColor *= textures[12].Sample(sampState, texCoords);
				break;
			case 14.0:
				texColor *= textures[13].Sample(sampState, texCoords);
				break;
			case 15.0:
				texColor *= textures[14].Sample(sampState, texCoords);
				break;
			case 16.0:
				texColor *= textures[15].Sample(sampState, texCoords);
				break;
			case 17.0:
				texColor *= textures[16].Sample(sampState, texCoords);
				break;
			case 18.0:
				texColor *= textures[17].Sample(sampState, texCoords);
				break;
			case 19.0:
				texColor *= textures[18].Sample(sampState, texCoords);
				break;
			case 20.0:
				texColor *= textures[19].Sample(sampState, texCoords);
				break;
			case 21.0:
				texColor *= textures[20].Sample(sampState, texCoords);
				break;
			case 22.0:
				texColor *= textures[21].Sample(sampState, texCoords);
				break;
			case 23.0:
				texColor *= textures[22].Sample(sampState, texCoords);
				break;
			case 24.0:
				texColor *= textures[23].Sample(sampState, texCoords);
				break;
			case 25.0:
				texColor *= textures[24].Sample(sampState, texCoords);
				break;
			case 26.0:
				texColor *= textures[25].Sample(sampState, texCoords);
				break;
			case 27.0:
				texColor *= textures[26].Sample(sampState, texCoords);
				break;
			case 28.0:
				texColor *= textures[27].Sample(sampState, texCoords);
				break;
			case 29.0:
				texColor *= textures[28].Sample(sampState, texCoords);
				break;
			case 30.0:
				texColor *= textures[29].Sample(sampState, texCoords);
				break;
			case 31.0:
				texColor *= textures[30].Sample(sampState, texCoords);
				break;
			case 32.0:
				texColor *= textures[31].Sample(sampState, texCoords);
				break;
		}
	}

	return texColor;
}
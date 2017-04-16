R"(

SamplerState samp {
	AddressU = Wrap;
	AddressV = Wrap;
	Filter = MIN_MAG_MIP_NEAREST;
};

#shaderGen define_b block1
Texture2D texvar : register(tsg_sub(var, 1));
#shaderGen endblock

#shaderGen generate for(var, 1, le, maxTextures, 1) block1

float4 psMain(float4 position : SV_POSITION, float2 texCoords : TEXCOORDS, float4 color : COLOR, float tid : TID) : SV_TARGET0 {
	float4 texColor = float4(1, 1, 1, 1);	

	switch(tid) {
	case 0:
		break;
#shaderGen define_b block2
	case var:
		texColor = texvar.Sample(samp, texCoords);
		break;
#shaderGen endblock

#shaderGen generate for(var, 1, le, maxTextures, 1) block2
	}

	return color * texColor;
}

)"
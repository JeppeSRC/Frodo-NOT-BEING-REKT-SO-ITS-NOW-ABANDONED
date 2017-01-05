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

float4 psMain(float4 position : SV_POSITION, float2 texCoord : TEXCOORDS, float4 color : COLOR, float tid : TID) : SV_TARGET0 {
	float alpha = 1;
	
	switch(tid) {
#shaderGen define_b case_block
		case var:
			alpha = texvar.Sample(samp, texCoord).x;
			break;
#shaderGen endblock

#shaderGen generate for(var, 1, le, maxTextures, 1) case_block
	}

	
	return color * alpha;
}

)"
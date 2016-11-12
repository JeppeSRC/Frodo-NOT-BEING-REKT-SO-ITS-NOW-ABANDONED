R"(

SamplerState samp {
	AddressU = Wrap;
	AddressV = Wrap;
	Filter = MIN_MAG_MIP_LINEAR;
};

Texture2D tex1 : register(t0);
Texture2D tex2 : register(t1);
Texture2D tex3 : register(t2);
Texture2D tex4 : register(t3);

float4 psMain(float4 position : SV_POSITION, float2 texCoord : TEXCOORDS, float tid : TID) : SV_TARGET0 {
	float alpha = 1;
	
	switch(tid) {
		case 1:
			alpha = tex1.Sample(samp, texCoord).x;
			break;
		case 2:
			alpha = tex2.Sample(samp, texCoord).x;
			break;
		case 3:
			alpha = tex3.Sample(samp, texCoord).x;
			break;
		case 4:
			alpha = tex4.Sample(samp, texCoord).x;
			break;
	}

	return float4(alpha, alpha, alpha, alpha);
}

)"
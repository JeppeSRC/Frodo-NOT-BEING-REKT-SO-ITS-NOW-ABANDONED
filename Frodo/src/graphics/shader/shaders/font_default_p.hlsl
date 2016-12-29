R"(

SamplerState samp {
	AddressU = Wrap;
	AddressV = Wrap;
	Filter = MIN_MAG_MIP_NEAREST;
};

#shaderGen define test2 u32 22
#shaderGen define test3 u32 23

#shaderGen if defined(test)

Defined

#shaderGen elif le(test2, test3)

Texture2D tex1 : register(t0);
Texture2D tex2 : register(t1);
Texture2D tex3 : register(t2);
Texture2D tex4 : register(t3);
Texture2D tex5 : register(t4);
Texture2D tex6 : register(t5);
Texture2D tex7 : register(t6);
Texture2D tex8 : register(t7);
Texture2D tex9 : register(t8);
Texture2D tex10 : register(t9);
Texture2D tex11 : register(t10);
Texture2D tex12 : register(t11);
Texture2D tex13 : register(t12);
Texture2D tex14 : register(t13);
Texture2D tex15 : register(t14);
Texture2D tex16 : register(t15);

#shaderGen elif defined(test3)

defined 3

#shaderGen else

Else

#shaderGen endif

float4 psMain(float4 position : SV_POSITION, float2 texCoord : TEXCOORDS, float4 color : COLOR, float tid : TID) : SV_TARGET0 {
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
		case 5:
			alpha = tex5.Sample(samp, texCoord).x;
			break;
		case 6:
			alpha = tex6.Sample(samp, texCoord).x;
			break;
		case 7:
			alpha = tex7.Sample(samp, texCoord).x;
			break;
		case 8:
			alpha = tex8.Sample(samp, texCoord).x;
			break;		
		case 9:
			alpha = tex9.Sample(samp, texCoord).x;
			break;
		case 10:
			alpha = tex10.Sample(samp, texCoord).x;
			break;
		case 11:
			alpha = tex11.Sample(samp, texCoord).x;
			break;
		case 12:
			alpha = tex12.Sample(samp, texCoord).x;
			break;		
		case 13:
			alpha = tex13.Sample(samp, texCoord).x;
			break;
		case 14:
			alpha = tex14.Sample(samp, texCoord).x;
			break;
		case 15:
			alpha = tex15.Sample(samp, texCoord).x;
			break;
		case 16:
			alpha = tex16.Sample(samp, texCoord).x;
			break;
	}

	
	return color * alpha;
}

)"
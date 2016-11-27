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

float4 psMain(float4 position : SV_POSITION, float2 texCoord : TEXCOORDS, float4 color : COLOR, float tid : TID) : SV_TARGET0 {
	float4 tex = float4(1, 1, 1, 1);
	
	switch(tid) {
		case 1:
			tex = tex1.Sample(samp, texCoord);
			break;
		case 2:
			tex = tex2.Sample(samp, texCoord);
			break;
		case 3:
			tex = tex3.Sample(samp, texCoord);
			break;
		case 4:
			tex = tex4.Sample(samp, texCoord);
			break;
		case 5:
			tex = tex5.Sample(samp, texCoord);
			break;
		case 6:
			tex = tex6.Sample(samp, texCoord);
			break;
		case 7:
			tex = tex7.Sample(samp, texCoord);
			break;
		case 8:
			tex = tex8.Sample(samp, texCoord);
			break;		
		case 9:
			tex = tex9.Sample(samp, texCoord);
			break;
		case 10:
			tex = tex10.Sample(samp, texCoord);
			break;
		case 11:
			tex = tex11.Sample(samp, texCoord);
			break;
		case 12:
			tex = tex12.Sample(samp, texCoord);
			break;		
		case 13:
			tex = tex13.Sample(samp, texCoord);
			break;
		case 14:
			tex = tex14.Sample(samp, texCoord);
			break;
		case 15:
			tex = tex15.Sample(samp, texCoord);
			break;
		case 16:
			tex = tex16.Sample(samp, texCoord);
			break;
	}


	
	return color * tex;
}

)"
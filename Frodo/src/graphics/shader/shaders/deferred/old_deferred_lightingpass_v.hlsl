R"(




struct Out  {
	float4 position : SV_POSITION;
	float2 texCoords : TEXCOORDS;
};

Out vsMain(float3 position : POSITION, float2 texCoords : TEXCOORDS) {
	
	Out o;
		
	o.position = float4(position, 1);
	o.texCoords = texCoords;

	return o;
}


)"
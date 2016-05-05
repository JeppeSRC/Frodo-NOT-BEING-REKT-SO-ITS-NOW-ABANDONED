struct Out {
	float4 position : SV_POSITION;
	float2 texCoords : TEXCOORDS;
};


Out vsMain(float4 position : POSITION, float2 texCoords : TEXCOORDS) {
	Out o;

	o.position = position;
	o.texCoords = texCoords;

	return o;
}
R"( 

SamplerState samp {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

cbuffer lightData : register(b0) {
	float3 l_color;
	float pad0;
	float3 l_direction;
	float pad1;
};

cbuffer materialData : register(b1) {
	float4 m_color;
};

Texture2D diffuseTexture : register(t0);

float4 psMain(float4 pos : SV_POSITION, float3 position : POSITION, float2 texCoord : TEXCOORDS, float3 normal : NORMALS) : SV_TARGET {

	float brightness = dot(normalize(normal), -l_direction);
	float4 diffuse = m_color * diffuseTexture.Sample(samp, texCoord);

	//return float4(position.xyz, 1);
	return float4(m_color.xyz * l_color * diffuse.xyz * brightness, 1);
}

)"
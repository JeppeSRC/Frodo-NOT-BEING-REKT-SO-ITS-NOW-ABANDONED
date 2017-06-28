
struct Light {
	float3 Color;
	float pad0;
	float3 Position;
	float pad1;
	float3 Attenuation;
	float pad2;
};

struct Out {
	float4 pos : SV_POSITION;
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float3 camPos : CAMPOS;
	Light light : LIGHT;
};

cbuffer Camera : register(b1) {
	float3 c_Position;
	float c_Pad0;
	float4x4 c_ViewMatrix;
	float4x4 c_ProjectionMatrix;
};

cbuffer Model : register(b5) {
	float4x4 m_ModelMatrix;
};

cbuffer Light : register(b6) {
	Light light;
};

Out vsMain(float3 position : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD, float3 tangent : TANGENT) {
	Out o;

/*	float3 T = normalize(mul(m_ModelMatrix, float4(tangent, 0.0)).xyz);
	float3 N = mul(m_ModelMatrix, float4(normal, 0)).xyz;
	
	T = normalize(T - (N * dot(T, N)));

	float3 B = cross(N, T);

	float3x3 TBN = transpose(float3x3(T, B, N));

	o.pos = mul(c_ProjectionMatrix, mul(c_ViewMatrix, mul(m_ModelMatrix, float4(position, 1))));
	o.position = mul(TBN, mul(m_ModelMatrix, float4(position, 0.0)).xyz);
	o.normal = N;
	o.texCoord = texCoord;
	o.camPos = mul(TBN, c_Position);
	o.light.Color = light.Color;
	o.light.Position = mul(TBN, light.Position);*/

	o.pos = mul(c_ProjectionMatrix, mul(c_ViewMatrix, mul(m_ModelMatrix, float4(position, 1))));
	o.position = mul(m_ModelMatrix, float4(position, 1)).xyz;
	o.normal = mul(m_ModelMatrix, float4(normal, 0)).xyz;
	o.texCoord = texCoord;
	o.camPos = c_Position;
	o.light.Color = light.Color;
	o.light.Position = light.Position;

	return o;
}
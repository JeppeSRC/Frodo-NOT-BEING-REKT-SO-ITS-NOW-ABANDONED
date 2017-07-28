R"(

float psMain(float4 position : SV_POSITION, float depth : DEPTH) : SV_Target {
	return depth;
}

)"
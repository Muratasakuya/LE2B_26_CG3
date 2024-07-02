
struct VertexShaderOutput {
    
	float4 position : POSITION;
};

/*===========================================================
                     Basic VS.GS  Shader
===========================================================*/

struct VertexShaderInput {
    
	float4 position : POSITION0;
};

VertexShaderOutput main(float4 pos : POSITION) {
    
	VertexShaderOutput output;
    
	output.position = pos;
    
	return output;
}
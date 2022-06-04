#version 430

in vec4 varyingColor;
in vec2 tc;
out vec4 color;


uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

uniform int textureSample = 1;
layout(binding = 0) uniform sampler2D samp;

void main(void)
{
	if(textureSample == 1)
	{
		color = texture(samp,tc);
	}
	else
	{
	   color = vec4(1.0,0.0,0.0,1.0);
	}

}

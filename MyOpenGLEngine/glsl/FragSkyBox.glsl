#version 420 
out vec4 FragColor;

in vec3 TexCoords;

layout(binding = 0) uniform samplerCube SkyBox;

void main()
{    
    FragColor = texture(SkyBox, TexCoords);
}
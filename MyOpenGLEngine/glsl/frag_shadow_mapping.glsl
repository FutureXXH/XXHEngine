#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;


void main()
{
    // ��ȡ���ߵ������
    float lightDistance = length(FragPos.xyz - lightPos);

    
    lightDistance = lightDistance / 100;

 
    gl_FragDepth = lightDistance;
}
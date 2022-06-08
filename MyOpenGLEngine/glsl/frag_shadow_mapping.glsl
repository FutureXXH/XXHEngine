#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;


void main()
{
    // 获取光线到面距离
    float lightDistance = length(FragPos.xyz - lightPos);

    
    lightDistance = lightDistance / 100;

 
    gl_FragDepth = lightDistance;
}
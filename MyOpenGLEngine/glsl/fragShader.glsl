#version 430

in vec2 tc;
out vec4 color;


in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;

uniform int textureSample = 1;
layout(binding = 0) uniform sampler2D samp;


void main(void)
{
    // ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    //float specularStrength = 0.5;
    float Shinienss = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(norm, normalize(lightDir+viewDir)), 0.0), Shinienss*30);  //B_P
   // float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shinienss*30); //P 
    vec3 specular =  spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) ;


	if(textureSample == 1)
	{

	
		color = texture(samp,tc)*vec4(result,1.0);
	}
	else
	{
	   color = vec4(1.0,1.0,1.0,1.0)*vec4(result,1.0);
	}

}

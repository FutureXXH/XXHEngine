#version 430


out vec4 color;


uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;

layout (binding = 0)uniform sampler2D diffuseTexture;
layout (binding = 1)uniform samplerCube  shadowMap;



uniform int textureSample = 1;


struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
}; 

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;

} fs_in;


uniform Material material;


float ShadowCalculation(vec3 fragPosLightSpace)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fs_in.FragPos - lightPos;
    // Use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(shadowMap, fragToLight).r;
    // It is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= 100;
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // Now test for shadows
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}



void main(void)
{



    // ambient
    float ambientStrength = 0.3;
    vec3 ambient = texture(material.diffuse,fs_in.TexCoords).rgb*ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = texture(material.diffuse,fs_in.TexCoords).rgb * diff * lightColor;
    
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(norm, normalize(lightDir+viewDir)), 0.0), material.shininess*30);  //B_P Ä£ÐÍ
   // float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shinienss*30); //P 
    vec3 specular =  spec * lightColor;    
        
   float shadow = ShadowCalculation(fs_in.FragPos);      

    color = vec4(ambient + (1.0 - shadow)*( diffuse + specular),1.0);


    
}

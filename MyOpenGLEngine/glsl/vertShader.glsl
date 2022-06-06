#version 430
mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x,float y,float z);


layout (location = 0)in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 vNormal;

out vec2 tc;
uniform mat4 v_matrix;
uniform mat4 m_matrix;

uniform mat4 proj_matrix;


layout (binding = 0) uniform sampler2D samp;


out vec3 FragPos;
out vec3 Normal;

void main(void)
{

  gl_Position = proj_matrix*v_matrix *m_matrix *vec4(position,1.0);
  tc = texCoord;

  FragPos = vec3(m_matrix * vec4(position, 1.0));
  Normal = mat3(transpose(inverse(m_matrix))) * vNormal;  
    


}

mat4 buildRotateX(float rad)
{

mat4 xrot = mat4(
1.0,0.0,0.0,0.0,
0.0,cos(rad),-sin(rad),0.0,
0.0,sin(rad),cos(rad),0.0,
0.0,0.0,0.0,1.0
);

return xrot;
}

mat4 buildRotateY(float rad)
{
mat4 yrot = mat4(
cos(rad),0.0,sin(rad),0.0,
0.0,1.0,0.0,0.0,
-sin(rad),0,cos(rad),0.0,
0.0,0.0,0.0,1.0
);

return yrot;
}

mat4 buildRotateZ(float rad)
{

mat4 zrot = mat4(
cos(rad),-sin(rad),0.0,0.0,
sin(rad),cos(rad),0.0,0.0,
0.0,0,1.0,0.0,
0.0,0.0,0.0,1.0
);

return zrot;
}



mat4 buildTranslate(float x,float y,float z)
{
mat4 trans = mat4(
1.0,0.0,0.0,0.0,
0.0,1.0,0.0,0.0,
0.0,0.0,1.0,0.0,
x,y,z,1.0
);
return trans;
}
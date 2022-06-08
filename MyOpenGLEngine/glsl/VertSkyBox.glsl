#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 m_matrix;
uniform mat4 v_matrix;
void main()
{

    TexCoords = aPos;
    mat4 v_matrix2 = mat4(mat3(v_matrix));
    gl_Position = projection * v_matrix2 * vec4(aPos, 1.0);


}  
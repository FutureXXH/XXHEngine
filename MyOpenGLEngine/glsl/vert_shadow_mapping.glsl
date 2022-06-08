#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 m_matrix;

void main()
{
    gl_Position = m_matrix * vec4(position, 1.0);
}
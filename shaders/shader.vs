#version 150 core

in vec3 position;
in vec3 v_color;
in vec3 normal;
out vec3 f_color;
uniform mat4 m_transform;
out vec3 normal0;

void main()
{
    //gl_Position = m_transform * vec4(position, 1.0);
    f_color = v_color;
    normal0 = (vec4(normal, 0.0)).xyz;
    //normal0 = (m_transform * vec4(normal, 0.0)).xyz;
    
    gl_Position = vec4(position, 1.0);
    //normal0 = normal;
}
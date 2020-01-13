#version 150 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec3 f_color[];
in vec3 normal0[];

out vec3 f_color2;
out vec3 normal1;

uniform mat4 m_transform;

void main()
{
    for (int i=0; i < gl_in.length(); i++)
    {
        f_color2 = f_color[i];
        normal1 = normal0[i];
        
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    
    EndPrimitive();
}
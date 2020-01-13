#version 150 core

layout(triangles) in;
//layout(line_strip, max_vertices=9) out;
layout(triangle_strip, max_vertices=3) out;

in vec3 f_color[];
in vec3 normal0[];

out vec3 f_color2;
out vec3 normal1;

uniform mat4 m_transform;

void main()
{
    //f_color2 = f_color[0];
    //normal1 = normal0[0];
    
    //    gl_Position = gl_in[].gl_Position;
    //    EmitVertex();
    
    for (int i=0; i < gl_in.length(); i++)
    {
        f_color2 = vec3(1.0,0.3,0.3);
        normal1 = (m_transform * vec4(normal0[i], 0.0)).xyz;
        
        gl_Position = m_transform * vec4(gl_in[i].gl_Position.xyz, 1.0);
        EmitVertex();
    }
    EndPrimitive();
    
    float normal_length = .1;
    for (int i=0; i < gl_in.length(); i++)
    {
        f_color2 = vec3(1.0,1.0,1.0);
        normal1 = (m_transform * vec4(normal0[i], 0.0)).xyz;
        
        gl_Position = m_transform * vec4(gl_in[i].gl_Position.xyz, 1.0);
        EmitVertex();
        
        gl_Position = m_transform * vec4(gl_in[i].gl_Position.xyz + normal0[i] * normal_length, 1.0);
        EmitVertex();
        
        EndPrimitive();
    }
    
    
    
    
    //gl_Position = gl_in[i].gl_Position;
    //EmitVertex();
    
    //gl_Position = gl_in[i].gl_Position + vec4(normal0[0], 1.0);
    //EmitVertex();
    
//    vec3 p1;
//    vec3 p2;
//    vec3 p3;
//    
//    vec3 N;
//    
    //        if ( i > 1 )
    //        {
    //    for (int i=0; i < gl_in.length(); i++)
    //    {
//    p1 = gl_in[0].gl_Position.xyz;
//    p2 = gl_in[1].gl_Position.xyz;
//    p3 = gl_in[2].gl_Position.xyz;
//    
//    N = normalize(cross(p2-p1, p3-p1));
//    
//    float normal_length = .5;
//    
//    gl_Position = m_transform * gl_in[0].gl_Position;
//    EmitVertex();
//    
//    gl_Position = m_transform * vec4(gl_in[0].gl_Position.xyz + N * normal_length, 0.0);
//    EmitVertex();
//    EndPrimitive();
//    
//    gl_Position = m_transform * gl_in[1].gl_Position;
//    EmitVertex();
//    
//    gl_Position = m_transform * vec4(gl_in[1].gl_Position.xyz + N * normal_length, 0.0);
//    EmitVertex();
//    EndPrimitive();
//    
//    gl_Position = m_transform * gl_in[2].gl_Position;
//    EmitVertex();
//    
//    gl_Position = m_transform * vec4(gl_in[2].gl_Position.xyz + N * normal_length, 0.0);
//    EmitVertex();
    //        }
    
    
//    EndPrimitive();
}
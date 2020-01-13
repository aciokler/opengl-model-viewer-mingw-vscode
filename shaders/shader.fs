#version 150 core


uniform vec4 color;
in vec3 f_color2;
in vec3 normal1;
//in vec3 f_color;
//in vec3 normal0;
out vec4 fragData;

void main()
{
    //fragData = vec4(0.5, 1.0, 0.5, 1.0);
    //fragData = vec4(color.x, color.y, color.z, 1.0) * clamp( dot(-vec3(0,0,1), normal0 ), 0.0, 1.0 );
    //fragData = vec4( vec3(normal0.x, normal0.y, normal0.z) * clamp( dot(-vec3(0,0,1), normal0 ), 0.0, 1.0), 1.0 );
    //fragData = vec4( vec3(1.0, 1.0, 1.0) * clamp( dot(-vec3(0,0,1), normal0 ), 0.0, 1.0 ), 1.0 );
    //fragData = vec4( vec3(1.0, 1.0, 1.0) * clamp( dot(-vec3(0,0,1), normal1 ), 0.0, 1.0 ), 1.0 );
    //fragData = vec4( vec3(normal1.x, normal1.y, normal1.z), 1.0 );
    //fragData = vec4( vec3(f_color2.x, f_color2.y, f_color2.z), 1.0 );
    
    const vec3 diffuseColor = vec3(0.9, 0.6, 0.6);
    const vec3 specColor = vec3(0.5, 0.5, 0.5);
    
    //PHONG brdf
    vec3 lightDir = -vec3(0,0,1);
    vec3 viewDirection = -lightDir;
    
    //float ambientSharing =
    float exponent = 4.0;
    float lambertianShading = clamp( dot(lightDir, normal1), 0.0, 1.0 );
    float phongBrdf = pow( clamp(dot(reflect(lightDir,normal1), viewDirection), 0.0, 1.0), exponent);
    
    fragData = vec4( lambertianShading * diffuseColor + phongBrdf * specColor, 1.0 );
}
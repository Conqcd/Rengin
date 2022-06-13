#version 450

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in int a_MaterialId;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;


void main()
{  
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_position,1.0);
}
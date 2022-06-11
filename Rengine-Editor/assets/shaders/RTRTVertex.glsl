#version 450

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in int a_MaterialId;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texCoords;
out float v_Depth;
flat out int v_MaterialId;

void main()
{  
    v_position = (u_Transform * vec4(a_position, 1.0)).xyz;
    v_normal = (u_Transform * vec4(a_normal, 0.0)).xyz;
    v_texCoords = a_texCoords;
    v_MaterialId = a_MaterialId;
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_position,1.0);
    v_Depth = gl_Position.w;
}
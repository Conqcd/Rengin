#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

uniform mat4 u_Transform;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_LightMVP; 


out mat4 v_WorldToScreen;
out vec4 v_PosWorld;

void main(void) 
{
    vec4 posWorld = u_Transform * vec4(a_position, 1.0);
    v_PosWorld = posWorld.xyzw / posWorld.w;
    v_WorldToScreen = u_Projection * u_View;

    gl_Position = u_Projection * u_View * u_Transform * vec4(a_position, 1.0);
}
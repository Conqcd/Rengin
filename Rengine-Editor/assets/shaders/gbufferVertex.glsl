#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

uniform mat4 u_LightVP;
uniform mat4 u_Transform;
uniform mat4 u_View;
uniform mat4 u_Projection;

out mat4 v_WorldToLight;

out vec2 v_TextureCoord;
out vec3 v_NormalWorld;
out vec4 v_PosWorld;
out float v_Depth;

void main(void) 
{
    vec4 posWorld = u_Transform * vec4(a_position, 1.0);
    v_PosWorld = posWorld.xyzw / posWorld.w;
    vec4 normalWorld = u_Transform * vec4(a_normal, 0.0);
    v_NormalWorld = normalize(normalWorld.xyz);
    v_TextureCoord = a_texCoords;
    v_WorldToLight = u_LightVP;

    gl_Position = u_Projection * u_View * u_Transform * vec4(a_position, 1.0);
    v_Depth = gl_Position.w;
}
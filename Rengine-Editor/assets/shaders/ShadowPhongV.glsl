#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;
uniform mat4 u_LightMVP;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texCoords;
out vec4 v_PositionFromLight;

void main()
{  
    v_position = (u_Transform * vec4(a_position, 1.0)).xyz;
    v_normal = (u_Transform * vec4(a_normal, 0.0)).xyz;
    v_texCoords = a_texCoords;
    v_PositionFromLight = u_LightMVP * vec4(a_position, 1.0);
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_position,1.0);
}
#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texCoords;

void main()
{
    v_position = a_position;
    v_normal = a_normal;
    v_texCoords = a_texCoords;
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_position,1.0);
}
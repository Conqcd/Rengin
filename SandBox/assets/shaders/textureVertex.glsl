#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoords;

void main()
{
    v_TexCoords = a_texCoords;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_position,1.0);
}
#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in float a_texIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_position;

void main()
{
    v_position = a_position;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_position,1.0);
}
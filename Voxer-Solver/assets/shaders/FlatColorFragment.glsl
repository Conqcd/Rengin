#version 330

in vec3 v_position;
uniform vec4 u_Color;
layout(location = 0) out vec4 color;

void main()
{
    color = u_Color;
}
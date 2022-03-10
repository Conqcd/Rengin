#version 330

in vec3 v_position;
layout(location = 0) out vec4 o_color;

void main()
{
    // color = vec4(v_position,1.0);
    o_color = vec4(1.0,1.0,1.0,1.0);
}
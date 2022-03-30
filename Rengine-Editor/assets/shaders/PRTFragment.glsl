#version 330


layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_Entity;
uniform int u_Entity;

in vec3 v_Color;

void main(void) {
    o_color = vec4(v_Color,1.0);
    o_Entity = u_Entity;
}
#version 450

in vec2 v_texCoords;

layout(location = 0) out vec4 o_color;

uniform sampler2D u_Screen;

void main()
{
    vec2 tex = v_texCoords;
    o_color = texture(u_Screen,tex);
}
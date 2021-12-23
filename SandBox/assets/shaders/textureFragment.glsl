#version 330

layout(location = 0) out vec4 color;


in vec2 v_TexCoords;
uniform sampler2D u_texture;
uniform float u_TilingFactor;
uniform vec4 u_color;

void main()
{
    color = texture2D(u_texture,v_TexCoords * u_TilingFactor) * u_color;
}
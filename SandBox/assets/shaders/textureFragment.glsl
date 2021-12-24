#version 330

layout(location = 0) out vec4 color;


in vec2 v_TexCoords;
in vec4 v_Color;
in float v_texIndex;

uniform sampler2D u_textures[32];
uniform float u_TilingFactor;
uniform vec4 u_color;

void main()
{
    color = texture2D(u_textures[int(v_texIndex)],v_TexCoords * u_TilingFactor) * u_color;
}
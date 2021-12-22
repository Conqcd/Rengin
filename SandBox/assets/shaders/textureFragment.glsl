#version 330

layout(location = 0) out vec4 color;


in vec2 v_TexCoords;
uniform sampler2D u_texture;

void main()
{
    color = texture2D(u_texture,v_TexCoords);
}
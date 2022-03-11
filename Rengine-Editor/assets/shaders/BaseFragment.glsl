#version 330

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texCoords;


layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_Entity;

uniform int u_Entity;
uniform sampler2D m_texture;

void main()
{
    o_color = texture2D(m_texture,v_texCoords);
    o_Entity = u_Entity;
}
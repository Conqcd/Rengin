#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in float a_texIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_TexCoords;
out flat float v_texIndex;
out float v_TilingFactor;
out flat int v_EntityID;

void main()
{
    v_Color = a_color;
    v_texIndex = a_texIndex;
    v_TexCoords = a_texCoords;
    v_TilingFactor = a_TilingFactor;
    v_EntityID = a_EntityID;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_position,1.0);
}
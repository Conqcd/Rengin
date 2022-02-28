#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

struct VertexOutput
{
    vec4 Color;
};

layout(location = 0) out VertexOutput Output;
layout(location = 1) out flat int v_EntityID;

void main()
{
    Output.Color = a_color;
    v_EntityID = a_EntityID;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_WorldPosition,1.0);
}
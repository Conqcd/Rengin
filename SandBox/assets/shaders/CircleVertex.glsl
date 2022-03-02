#version 410 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_color;
layout(location = 3) in float a_thickness;
layout(location = 4) in float a_fade;
layout(location = 5) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

struct VertexOutput
{
    vec3 LocalPosition;
    vec4 Color;
    float Thickness;
    float Fade;
};

layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int v_EntityID;

void main()
{
    Output.Color = a_color;
    Output.LocalPosition = a_LocalPosition;
    Output.Thickness = a_thickness;
    Output.Fade = a_fade;
    v_EntityID = a_EntityID;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_WorldPosition,1.0);
}
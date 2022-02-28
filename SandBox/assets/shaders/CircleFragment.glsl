#version 330

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_EntityID;

struct VertexInput
{
    vec3 LocalPosition;
    vec4 Color;
    float Thickness;
    float Fade;
};

layout(location = 0) in VertexInput Input;
layout(location = 3) in flat int v_EntityID;

void main()
{
    float distance = 1.0 - length(Input.LocalPosition);
    vec3 color = vec3(smoothstep(0.0,Input.Fade,distance));
    o_color *= vec3(smoothstep(Input.Thickness + Input.Fade,Input.Thickness,distance));
    o_color *= Input.Color;

    o_EntityID = v_EntityID;
}

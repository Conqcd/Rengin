#version 410 core

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
layout(location = 4) in flat int v_EntityID;

void main()
{
    float distance = 1.0 - length(Input.LocalPosition);
    float circleAlpha = smoothstep(0.0,Input.Fade,distance);
    circleAlpha *= smoothstep(Input.Thickness + Input.Fade,Input.Thickness,distance);
    if(circleAlpha == 0.0)
        discard;
    
    o_color = Input.Color;
    o_color.a *= circleAlpha;

    o_EntityID = v_EntityID;
}

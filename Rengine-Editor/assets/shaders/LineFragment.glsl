#version 330

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_EntityID;

struct VertexInput
{
    vec4 Color;
};

layout(location = 0) in VertexInput Input;
layout(location = 1) in flat int v_EntityID;

void main()
{
    o_color = Input.Color;

    o_EntityID = v_EntityID;
}

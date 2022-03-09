#version 330

in vec3 v_position;
out vec4 color;

void main()
{
    color = vec4(v_position,1.0);
}
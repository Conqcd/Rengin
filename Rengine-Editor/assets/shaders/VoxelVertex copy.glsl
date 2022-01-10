#version 330 core
layout (location = 0) in vec3 a_position;

uniform mat4 u_TransformViewProjection;

void main()
{
    gl_Position = u_TransformViewProjection * vec4(a_position, 1.0);
    // if(a_position.x == 1.0)
    gl_Position = vec4(a_position, 1.0);
    // gl_PointSize = 10.0;
}

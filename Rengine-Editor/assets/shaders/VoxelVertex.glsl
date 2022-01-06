#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 ModelViewProjectionMatrix;

void main()
{
    gl_Position = ModelViewProjectionMatrix * vec4(aPos, 1.0);
    gl_PointSize = 10.0;
}

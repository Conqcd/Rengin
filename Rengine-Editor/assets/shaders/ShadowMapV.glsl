#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

uniform mat4 u_LightMVP;

void main(void) {

  gl_Position = u_LightMVP * vec4(a_position, 1.0);
}
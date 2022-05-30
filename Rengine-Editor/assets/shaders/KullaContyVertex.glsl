#version 450

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

uniform mat4 u_Transform;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 v_TextureCoord;
out vec3 v_FragPos;
out vec3 v_Normal;

void main(void) {
    v_FragPos = (u_Transform * vec4(a_position, 1.0)).xyz;
    v_Normal = (u_Transform * vec4(a_normal, 0.0)).xyz;

    gl_Position = u_Projection * u_View * u_Transform * vec4(a_position, 1.0);
    v_TextureCoord = a_texCoords;
}
#version 330

layout(location = 0) vec3 a_Position;
layout(location = 1) vec3 a_Normal;
layout(location = 2) vec2 a_TexCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_MoveWithCamera;

out vec2 v_TexCoords;
out vec3 v_FragPos;
out vec3 v_Normal;

void main() {

    v_Normal = a_Normal;
    v_TexCoords = a_TexCoords;
    mat4 viewMatrix = u_ViewMatrix;
    viewMatrix = mat4(mat3(viewMatrix));
     gl_Position = u_ProjectionMatrix * viewMatrix * u_ModelMatrix * u_MoveWithCamera * vec4(a_Position, 1.0);
    //gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aVertexPosition, 1.0);
    v_FragPos = a_Position;//gl_Position.xyz;
}
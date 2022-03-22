#version 330

uniform samplerCube skybox;

in vec2 v_TexCoords;
in vec3 v_FragPos;
in vec3 v_Normal;

void main() {
     gl_FragColor = textureCube(skybox, v_FragPos);
     //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
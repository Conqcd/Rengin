#version 330

uniform samplerCube u_skybox;

in vec3 v_TexCoords;
// in vec3 v_Normal;
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_Entity;
layout(location = 2) out vec3 o_Test;

void main() {
     o_Color = texture(u_skybox, v_TexCoords);
     o_Test = vec3(0.0);
     // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
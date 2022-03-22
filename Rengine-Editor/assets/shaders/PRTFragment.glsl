#version 330


uniform int u_Entity;

in vec4 v_Color;

void main(void) {
    gl_FragColor = v_Color;
}
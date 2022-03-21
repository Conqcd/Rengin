#version 330

// varying vec4 vPositionFromLight;
in vec4 v_Color;

void main(void) {
    gl_FragColor = v_Color;
}
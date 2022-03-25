#version 330

layout(location = 0) in vec3 a_position;
// layout(location = 1) in vec3 a_normal;
// layout(location = 2) in vec2 a_texCoords;
layout(location = 1) mat3 a_PrecomputeLT;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;

uniform mat3 u_PrecomputeLr;
uniform mat3 u_PrecomputeLg;
uniform mat3 u_PrecomputeLb;

out highp vec4 v_Color;

void main(void) {
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_position, 1.0);

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            v_Color.x += u_PrecomputeLr[i][j] * a_PrecomputeLT[i][j];
            v_Color.y += u_PrecomputeLg[i][j] * a_PrecomputeLT[i][j];
            v_Color.z += u_PrecomputeLb[i][j] * a_PrecomputeLT[i][j];
        }
    }
}
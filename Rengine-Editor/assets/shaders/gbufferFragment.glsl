#version 330

uniform sampler2D u_Kd;
uniform sampler2D u_Nt;
uniform sampler2D u_ShadowMap;

in mat4 v_WorldToLight;
in vec2 v_TextureCoord;
in vec4 v_PosWorld;
in vec3 v_NormalWorld;
in float v_Depth;

float SimpleShadowMap(vec3 posWorld,float bias){
    vec4 posLight = v_WorldToLight * vec4(posWorld, 1.0);
    vec2 shadowCoord = clamp(posLight.xy * 0.5 + 0.5, vec2(0.0), vec2(1.0));
    float depthSM = texture2D(u_ShadowMap, shadowCoord).x;
    float depth = (posLight.z * 0.5 + 0.5) * 100.0;
    return step(0.0, depthSM - depth + bias);
}

void LocalBasis(vec3 n, out vec3 b1, out vec3 b2) {
    float sign_ = sign(n.z);
    if (n.z == 0.0) {
      sign_ = 1.0;
    }
    float a = -1.0 / (sign_ + n.z);
    float b = n.x * n.y * a;
    b1 = vec3(1.0 + sign_ * n.x * n.x * a, sign_ * b, -sign_ * n.x);
    b2 = vec3(b, sign_ + n.y * n.y * a, -n.y);
}

vec3 ApplyTangentNormalMap() {
    vec3 t, b;
    LocalBasis(v_NormalWorld, t, b);
    vec3 nt = texture2D(u_Nt, v_TextureCoord).xyz * 2.0 - 1.0;
    nt = normalize(nt.x * t + nt.y * b + nt.z * v_NormalWorld);
    return nt;
}

void main(void) {
    vec3 kd = texture2D(u_Kd, v_TextureCoord).rgb;
    gl_FragData[0] = vec4(kd, 1.0);
    gl_FragData[1] = vec4(vec3(v_Depth), 1.0);
    gl_FragData[2] = vec4(ApplyTangentNormalMap(), 1.0);
    gl_FragData[3] = vec4(vec3(SimpleShadowMap(v_PosWorld.xyz, 1e-2)), 1.0);
    gl_FragData[4] = vec4(vec3(v_PosWorld.xyz), 1.0);
}

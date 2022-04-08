#version 330

uniform sampler2D u_Kd;
uniform sampler2D u_Nt;
uniform sampler2D u_ShadowMap;

in mat4 v_WorldToLight;
in vec2 v_TextureCoord;
in vec4 v_PosWorld;
in vec3 v_NormalWorld;
in float v_Depth;

layout(location = 0) out vec3 o_Kd;
layout(location = 1) out float o_Depth;
layout(location = 2) out vec3 o_NormalMap;
layout(location = 3) out float o_SimpleShadowMap;
layout(location = 4) out vec3 o_Position;

float SimpleShadowMap(vec3 posWorld,float bias)
{
    vec4 posLight = v_WorldToLight * vec4(posWorld, 1.0);
    vec2 shadowCoord = clamp(posLight.xy * 0.5 + 0.5, vec2(0.0), vec2(1.0));
    float depthSM = texture2D(u_ShadowMap, shadowCoord).x;
    float depth = (posLight.z * 0.5 + 0.5) * 100.0;
    return step(0.0, depthSM - depth + bias);
}

void LocalBasis(vec3 n, out vec3 b1, out vec3 b2) 
{
    float sign_ = sign(n.z);
    if (n.z == 0.0) {
      sign_ = 1.0;
    }
    float a = -1.0 / (sign_ + n.z);
    float b = n.x * n.y * a;
    b1 = vec3(1.0 + sign_ * n.x * n.x * a, sign_ * b, -sign_ * n.x);
    b2 = vec3(b, sign_ + n.y * n.y * a, -n.y);
}

vec3 ApplyTangentNormalMap()
{
    vec3 t, b;
    LocalBasis(v_NormalWorld, t, b);
    vec3 nt = texture2D(u_Nt, v_TextureCoord).xyz * 2.0 - 1.0;
    nt = normalize(nt.x * t + nt.y * b + nt.z * v_NormalWorld);
    return nt;
}

void main(void) 
{
    o_Kd = texture2D(u_Kd, v_TextureCoord).rgb;
    o_Depth = v_Depth;
    o_NormalMap = ApplyTangentNormalMap();
    o_SimpleShadowMap = SimpleShadowMap(v_PosWorld.xyz, 1e-2);
    o_Position = v_PosWorld.xyz;
}

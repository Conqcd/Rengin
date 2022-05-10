#version 450

uniform vec3 u_LightDir;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;
uniform vec3 u_LightRadiance;
uniform vec2 u_WindowSize;

uniform sampler2D u_GDiffuse;
uniform sampler2D u_GDepth;
uniform sampler2D u_GNormalWorld;
uniform sampler2D u_GShadow;
uniform sampler2D u_GPosWorld;

uniform int u_Entity;

in mat4 v_WorldToScreen;
in vec4 v_PosWorld;

// Out
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_Entity;

#define M_PI 3.1415926535897932384626433832795
#define TWO_PI 6.283185307
#define INV_PI 0.31830988618
#define INV_TWO_PI 0.15915494309

float Rand1(inout float p)
{
    p = fract(p * .1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}

vec2 Rand2(inout float p)
{
    return vec2(Rand1(p), Rand1(p));
}

float InitRand(vec2 uv)
{
	vec3 p3  = fract(vec3(uv.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

vec3 SampleHemisphereUniform(inout float s, out float pdf)
{
    vec2 uv = Rand2(s);
    float z = uv.x;
    float phi = uv.y * TWO_PI;
    float sinTheta = sqrt(1.0 - z * z);
    vec3 dir = vec3(sinTheta * cos(phi), sinTheta * sin(phi), z);
    pdf = INV_TWO_PI;
    return dir;
}

vec3 SampleHemisphereCos(inout float s, out float pdf)
{
    vec2 uv = Rand2(s);
    float z = sqrt(1.0 - uv.x);
    float phi = uv.y * TWO_PI;
    float sinTheta = sqrt(uv.x);
    vec3 dir = vec3(sinTheta * cos(phi), sinTheta * sin(phi), z);
    pdf = z * INV_PI;
    return dir;
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

vec4 Project(vec4 a)
{
    return a / a.w;
}

float GetDepth(vec3 posWorld)
{
    float depth = (v_WorldToScreen * vec4(posWorld, 1.0)).w;
    return depth;
}

/*
 * Transform point from world space to screen space([0, 1] x [0, 1])
 *
 */
vec2 GetScreenCoordinate(vec3 posWorld)
{
    vec2 uv = Project(v_WorldToScreen * vec4(posWorld, 1.0)).xy * 0.5 + 0.5;
    return uv;
}

float GetGBufferDepth(vec2 uv)
{
    float depth = texture2D(u_GDepth, uv).x;
    if (depth < 1e-2) {
        depth = 1000.0;
    }
    return depth;
}

vec3 GetGBufferNormalWorld(vec2 uv)
{
    vec3 normal = texture2D(u_GNormalWorld, uv).xyz;
    return normal;
}

vec3 GetGBufferPosWorld(vec2 uv)
{
    vec3 posWorld = texture2D(u_GPosWorld, uv).xyz;
    return posWorld;
}

float GetGBufferuShadow(vec2 uv)
{
    float visibility = texture2D(u_GShadow, uv).x;
    return visibility;
}

vec3 GetGBufferDiffuse(vec2 uv)
{
    vec3 diffuse = texture2D(u_GDiffuse, uv, 0).xyz;
    diffuse = pow(diffuse, vec3(2.2));
    return diffuse;
}

vec3 EvalDiffuse(vec3 wi, vec3 wo, vec2 uv)
{
    vec3 L = GetGBufferDiffuse(uv) * max(0,dot(normalize(-wi),normalize(GetGBufferNormalWorld(uv))));// * INV_PI;
    return L;
}

vec3 EvalDirectionalLight(vec2 uv)
{
    vec3 light_atten_coff = u_LightRadiance / pow(length(u_LightPos - v_PosWorld.xyz), 2.0);
    vec3 Le = light_atten_coff * GetGBufferuShadow(uv);
    return Le;
}

bool RayMarch(vec3 ori, vec3 dir, out vec3 hitPos)
{
    vec3 sOri,sDir;
    sOri = (v_WorldToScreen * vec4(ori,1.0)).xyz;
    sDir = (v_WorldToScreen * vec4(dir,0.0)).xyz;
    if(sDir.x == 0.0 && sDir.y == 0.0)
    {
        hitPos = sOri;
        return false;
    }
    bool swapXY = false;
    float dx = 1.0 / u_WindowSize.x,dy = dx * sDir.y / sDir.x * u_WindowSize.x / u_WindowSize.y,dz = sDir.z / sDir.x * dx;
    if(abs(sDir.y) * u_WindowSize.x > abs(sDir.x) * u_WindowSize.y)
    {
        swapXY = true;
        dy = 1.0 / u_WindowSize.y;
        dx = dy * sDir.x / sDir.y * u_WindowSize.y / u_WindowSize.x;
        dz = dy * sDir.z / sDir.y;
    }
    vec3 dP = vec3(dx,dy,dz);
    vec3 uvw = sOri;
    for(int i = 0;;i++)
    {
        uvw += dP;
        float depth = GetGBufferDepth(uvw.xy);
        if(fract(uvw.xy) != uvw.xy)
            return false;
        if(depth < uvw.z)
        {
            hitPos = GetGBufferPosWorld(uvw.xy);
            return true;
        }
    }
    return false;
}

#define SAMPLE_NUM 10

void main()
{
    float s = InitRand(gl_FragCoord.xy);

    vec2 uv = GetScreenCoordinate(v_PosWorld.xyz);
    vec3 ori = v_PosWorld.xyz;
    vec3 L = EvalDirectionalLight(uv) * EvalDiffuse(u_LightDir,ori - u_CameraPos,uv);
    vec3 Lindirect = vec3(0.0);
    vec3 b1,b2,b3 = normalize(GetGBufferNormalWorld(uv));
    LocalBasis(b3,b1,b2);
    for(int i = 0; i < SAMPLE_NUM;i++)
    {
        vec3 hitPos = vec3(0.0);
        float pdf = 0.0;
        // vec3 dir = SampleHemisphereUniform(s,pdf);
        vec3 dir = dot(b3,u_CameraPos - ori) * 2 / length(b3) * b3 + ori - u_CameraPos;
        // dir = dir.x * b1 + dir.y * b2 + dir.z * b3;
        if(RayMarch(ori,dir,hitPos))
        {
            vec2 uvi = GetScreenCoordinate(hitPos);
            Lindirect += EvalDiffuse(ori - hitPos,ori - u_CameraPos,uv) / pdf * EvalDirectionalLight(uvi) * EvalDiffuse(u_LightDir,hitPos - ori,uvi);
        }
    }
    L += Lindirect / SAMPLE_NUM;
    vec3 color = pow(clamp(L, vec3(0.0), vec3(1.0)), vec3(1.0 / 2.2));
    o_Color = vec4(vec3(color.rgb), 1.0);
    o_Entity = u_Entity;
}

#version 450
#define MAXBOUNCE 5
#define EPS 1e-3
#define M_PI 3.1415926535897932384626433832795
#define TWO_PI 6.283185307
#define INV_PI 0.31830988618
#define INV_TWO_PI 0.15915494309



in vec3 v_position;
in vec3 v_normal;
in vec2 v_texCoords;
flat in int v_MaterialId;

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_Entity;
layout(location = 2) out ivec4 o_T1;
layout(location = 3) out ivec4 o_T2;

uniform int u_Entity;
uniform int u_Bounce;
uniform sampler2D u_texture[32];
uniform int u_TexId;
uniform int u_trianglenums;
uniform float u_TimeSeed;

//Camera
uniform vec3 u_CameraPos;

// Light
uniform int u_LightNums;
uniform vec3 u_LightPos;
uniform vec3 u_LightIntensity;

//Material
uniform vec3 u_Ks;
uniform vec3 u_Kd;
uniform vec3 u_Ka;
uniform float u_Ns;
uniform float u_Ni;

struct Triangle
{
    float Vertex[3];
    float Normal[3];
    float Texture[2];
};

struct Material
{
    float Ks[3];
    float Kd[3];
    float Ka[3];
    float Le[3];
    float Ns;
    float Ni;
    // bool is_specular;
};

// Triangle
layout(std430,binding = 0) buffer Triangles
{
    Triangle m_Vertex[];
};

layout(std430,binding = 1) buffer MaterialId
{
    int m_MId[];
};

// Triangle Index
layout(std430,binding = 2) buffer Indices
{
    int m_index[];
};

// Material
layout(std430,binding = 3) buffer Materials
{
    Material m_Materials[];
};

// Material ID
layout(std430,binding = 4) buffer LightID
{
    int m_LightsID[];
};

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

vec3 Rand3(inout float p)
{
    return vec3(Rand1(p), Rand2(p));
}

vec4 Rand4(inout float p)
{
    return vec4(Rand2(p), Rand2(p));
}

float TriangleArea(vec3 v1,vec3 v2,vec3 v3)
{
    return length(cross(v1 - v2,v1 - v3)) / 2;
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

vec3 SampleHemisphereUniform(inout float s, out float pdf,float ns)
{
    vec2 uv = Rand2(s);
    float z = uv.x;
    float phi = uv.y * TWO_PI;
    float roughness = 1 / ns;
    float Theta = atan(roughness * roughness * sqrt(z) / sqrt(1.0 - z));
    vec3 dir = vec3(sin(Theta) * cos(phi), sin(Theta) * sin(phi), cos(Theta));
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

float InitRand(vec2 uv)
{
	vec3 p3  = fract(vec3(uv.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

vec3 ReflectBaseOnMaterial(vec3 normal,vec3 inLight,vec3 kd,vec3 ks,float ns,inout float s,inout float pdf)
{
    if(length(kd) == 0)
    {
        return reflect(normal,inLight);
    }else if(length(ks) == 0)
    {
        vec3 b1,b2,b3 = normalize(normal);
        LocalBasis(b3,b1,b2);
        vec3 dir = SampleHemisphereUniform(s,pdf,1);
        dir = dir.x * b1 + dir.y * b2 + dir.z * b3;
        return dir;
    }else
    {
        float decide = Rand1(s);
        float kdl = 0.3 * kd.r + 0.6 * kd.g + 0.1 * kd.b;
        float ksl = 0.3 * ks.r + 0.6 * ks.g + 0.1 * ks.b;
        if(decide < kdl * (kdl + ksl))
        {
            vec3 b1,b2,b3 = normalize(normal);
            LocalBasis(b3,b1,b2);
            vec3 dir = SampleHemisphereUniform(s,pdf,1);
            dir = dir.x * b1 + dir.y * b2 + dir.z * b3;
            return dir;
        }else
        {
            vec3 b1,b2,b3 = normalize(reflect(normal,inLight));
            LocalBasis(b3,b1,b2);
            vec3 dir = SampleHemisphereUniform(s,pdf,ns);
            dir = dir.x * b1 + dir.y * b2 + dir.z * b3;
            return dir;
        }
   }
}

bool RayTriangleIntersect(vec3 position,vec3 direction,inout float t,int id,out vec3 oNormal,out vec3 hitpos)
{
    int id1 = m_index[id * 3],id2 = m_index[id * 3 + 1],id3 = m_index[id * 3 + 2];
    vec3 v0 = vec3(m_Vertex[id1].Vertex[0],m_Vertex[id1].Vertex[1],m_Vertex[id1].Vertex[2]);
    vec3 v1 = vec3(m_Vertex[id2].Vertex[0],m_Vertex[id2].Vertex[1],m_Vertex[id2].Vertex[2]);
    vec3 v2 = vec3(m_Vertex[id3].Vertex[0],m_Vertex[id3].Vertex[1],m_Vertex[id3].Vertex[2]);

	vec3 E1 = v1 - v0;
	vec3 E2 = v2 - v0;
	vec3 S = position - v0;
	vec3 S1 = cross(direction, E2);
	vec3 S2 = cross(S, E1);
	float coeff = 1.0 / dot(S1, E1);
	float tt = coeff * dot(S2, E2);
	float b1 = coeff * dot(S1, S);
	float b2 = coeff * dot(S2, direction);
	float b0 = 1 - b1 - b2;
    t = tt;
	if (b1 >= 0 && b2 >= 0 && b0 >= 0)
	{
        hitpos = v0 * b0 + v1 * b1 + v2 * b2;
        vec3 n0 = vec3(m_Vertex[id1].Normal[0],m_Vertex[id1].Normal[1],m_Vertex[id1].Normal[2]);
        vec3 n1 = vec3(m_Vertex[id2].Normal[0],m_Vertex[id2].Normal[1],m_Vertex[id2].Normal[2]);
        vec3 n2 = vec3(m_Vertex[id3].Normal[0],m_Vertex[id3].Normal[1],m_Vertex[id3].Normal[2]);

        oNormal = n0 * b0 + n1 * b1 + n2 * b2;
        return true;
	}
	return false;
}

bool RayTriangleIntersect_Easy(vec3 position,vec3 direction,inout float t,int id)
{
    int id1 = m_index[id * 3],id2 = m_index[id * 3 + 1],id3 = m_index[id * 3 + 2];
    vec3 v0 = vec3(m_Vertex[id1].Vertex[0],m_Vertex[id1].Vertex[1],m_Vertex[id1].Vertex[2]);
    vec3 v1 = vec3(m_Vertex[id2].Vertex[0],m_Vertex[id2].Vertex[1],m_Vertex[id2].Vertex[2]);
    vec3 v2 = vec3(m_Vertex[id3].Vertex[0],m_Vertex[id3].Vertex[1],m_Vertex[id3].Vertex[2]);

	vec3 E1 = v1 - v0;
	vec3 E2 = v2 - v0;
	vec3 S = position - v0;
	vec3 S1 = cross(direction, E2);
	vec3 S2 = cross(S, E1);
	float coeff = 1.0 / dot(S1, E1);
	float tt = coeff * dot(S2, E2);
	float b1 = coeff * dot(S1, S);
	float b2 = coeff * dot(S2, direction);
	float b0 = 1 - b1 - b2;
    t = tt;
	if (b1 >= 0 && b2 >= 0 && b0 >= 0)
	{
        return true;
	}
	return false;
}

bool hit(float t_min,float t_max,vec3 position,vec3 direction,out vec3 oNormal,out vec3 hitpos,out int hitMatId)
{
    float t = 0,min_t = t_max;
    vec3 normal,hitposition;
    int hitMaterialId;
    bool ishit = false;
    for(int i = 0; i < u_trianglenums; i++) {
        if(RayTriangleIntersect(position,direction,t,i,normal,hitposition) && t < min_t && t >= t_min)
        {
            min_t = t;
            oNormal = normal;
            hitpos = hitposition;
            hitMaterialId = m_MId[m_index[i * 3]];
            ishit = true;
        }
    }
    hitMatId = hitMaterialId;
    return ishit && dot(oNormal,direction) < 0;
}

bool hit_light(float t_min,float t_max,vec3 position,vec3 direction,float lightt)
{
    float t = 0,min_t = t_max;
    for(int i = 0; i < u_trianglenums; i++) {
        if(RayTriangleIntersect_Easy(position,direction,t,i) && t < min_t && t >= t_min)
            min_t = t;
    }
    return abs(lightt - min_t) < EPS;
}

vec3 light_color(vec3 ray_dir,vec3 ray_point,vec3 normal,vec3 ks,vec3 kd,float ns,inout float s)
{
    vec3 color = vec3(0.0);
    int i = int(Rand1(s) * u_LightNums);

    vec3 uv;
    uv.xy = Rand2(s);
    uv.y *= (1 - uv.x);
    uv.z = 1 - uv.x - uv.y;
    int id1 = m_index[m_LightsID[i] * 3],id2 = m_index[m_LightsID[i] * 3 + 1],id3 = m_index[m_LightsID[i] * 3 + 2];
    vec3 LV1 = vec3(m_Vertex[id1].Vertex[0],m_Vertex[id1].Vertex[1],m_Vertex[id1].Vertex[2]);
    vec3 LV2 = vec3(m_Vertex[id2].Vertex[0],m_Vertex[id2].Vertex[1],m_Vertex[id2].Vertex[2]);
    vec3 LV3 = vec3(m_Vertex[id3].Vertex[0],m_Vertex[id3].Vertex[1],m_Vertex[id3].Vertex[2]);
    
    vec3 LE1 = vec3(m_Materials[m_MId[id1]].Le[0],m_Materials[m_MId[id1]].Le[1],m_Materials[m_MId[id1]].Le[2]);
    vec3 LE2 = vec3(m_Materials[m_MId[id2]].Le[0],m_Materials[m_MId[id2]].Le[1],m_Materials[m_MId[id2]].Le[2]);
    vec3 LE3 = vec3(m_Materials[m_MId[id3]].Le[0],m_Materials[m_MId[id3]].Le[1],m_Materials[m_MId[id3]].Le[2]);
    
    vec3 LV = LV1 * uv.x + LV2 * uv.y + LV3 * uv.z;
    vec3 LE = LE1 * uv.x + LE2 * uv.y + LE3 * uv.z;

    vec3 Rdir = LV - ray_point;

    if(!hit_light(0.01,10000,ray_point,normalize(Rdir),length(Rdir)))
        return color;
    

    float distance_s = length(Rdir)* length(Rdir);
    float area = TriangleArea(LV1,LV2,LV3);
    float pdf = distance_s / area; 
    float pdf2 = max(dot(normalize(Rdir),normalize(normal)),0.0); 

    vec3 halfDir = normalize((Rdir + ray_dir));
    float spec = pow(max(dot(halfDir, normal), 0.0), ns);
    color += LE * pdf2 / pdf * (kd + ks * spec);
    return color;
}

vec3 ray_tracing(vec3 position,vec3 direction,vec3 normal,vec3 ks,vec3 kd,float ns,inout float s,inout vec3 debug)
{
    vec3 albedo[MAXBOUNCE];
    vec3 emit[MAXBOUNCE];
    vec3 color = vec3(0.0);
    int actI = 0;
    for(int i = 0;i < u_Bounce ;i++)
    {
        float pdf;
        vec3 dir = ReflectBaseOnMaterial(normal,direction,kd,ks,ns,s,pdf);
        vec3 hitpos;
        int hitMatId;
        vec3 oNormal;
        if(!hit(0.01,10000,position,dir,oNormal,hitpos,hitMatId))
            break;
        debug = hitpos * 10;
        vec3 Ks = vec3(m_Materials[hitMatId].Ks[0],m_Materials[hitMatId].Ks[1],m_Materials[hitMatId].Ks[2]);
        vec3 Kd = vec3(m_Materials[hitMatId].Kd[0],m_Materials[hitMatId].Kd[1],m_Materials[hitMatId].Kd[2]);

        vec3 Le = vec3(m_Materials[hitMatId].Le[0],m_Materials[hitMatId].Le[1],m_Materials[hitMatId].Le[2]);
        if(length(Le) > 0.0)
        {
            emit[i] = Le;
            break;
        }
        else
        {
            vec3 halfDir = normalize((dir - direction));
            float spec = pow(max(dot(halfDir, normal), 0.0), ns);
            float cosine = max(dot(normalize(dir),normalize(normal)),0.0);
            emit[i] = light_color(dir,hitpos,oNormal,Ks,Kd,m_Materials[hitMatId].Ns,s);
            albedo[i] = (kd + ks * spec) * cosine / pdf;
        }
        actI++;
        position = hitpos;
        direction = dir;
        normal = oNormal;
        ks = Ks;
        kd = Kd;
        ns = m_Materials[hitMatId].Ns;
    }
    for(int i = actI - 1;i >= 0 ;i--)
    {
        color = (color + emit[i]) * albedo[i];
    }
    return color;
}

void main()
{
    float s = InitRand(gl_FragCoord.xy) + u_TimeSeed;
    vec3 color = vec3(0.0);
    if(dot(v_normal,u_CameraPos - v_position) > 0.0)
    {
        vec3 Ks = vec3(m_Materials[v_MaterialId].Ks[0],m_Materials[v_MaterialId].Ks[1],m_Materials[v_MaterialId].Ks[2]);
        vec3 Kd = vec3(m_Materials[v_MaterialId].Kd[0],m_Materials[v_MaterialId].Kd[1],m_Materials[v_MaterialId].Kd[2]);

        vec3 Le = vec3(m_Materials[v_MaterialId].Le[0],m_Materials[v_MaterialId].Le[1],m_Materials[v_MaterialId].Le[2]);
        if(length(Le) > 0.0)
            color = Le;
        else
        {
            vec3 debug;
            color = light_color(u_CameraPos - v_position,v_position,v_normal,Ks,Kd,m_Materials[v_MaterialId].Ns,s);
            color += ray_tracing(v_position,u_CameraPos - v_position,v_normal,Ks,Kd,m_Materials[v_MaterialId].Ns,s,debug);
            o_T1.x = int(debug.x);
            o_T1.y = int(debug.y);
            o_T1.z = int(debug.z);
        }
        
    }
    o_color = vec4(color,1.0);
    o_Entity = u_Entity;
    // o_Entity = m_LightsID[0];
}
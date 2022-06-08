#version 450
#define MAXBOUNCE 5

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texCoords;

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_Entity;

uniform int u_Entity;
uniform int u_Bounce;
uniform sampler2D u_texture[32];
uniform int u_TexId;
uniform int u_trianglenums;

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
    float Texture[3];
};

struct Material
{
    float Ks[3];
    float Kd[3];
    bool is_specular;
};

struct Light
{
    float Vertex[3];
    float Le[3];
};

// Triangle
layout(std430,binding = 1) buffer Triangles
{
    Triangle m_Vertex[];
};

// Triangle Index
layout(std430,binding = 2) buffer Triangles
{
    Triangle m_Vertex[];
};

// Light Source
layout(std430,binding = 3) buffer Lights
{
    Light m_Lights[];
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

float InitRand(vec2 uv)
{
	vec3 p3  = fract(vec3(uv.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

vec3 BlinnPhong()
{
    vec3 color = texture2D(u_texture[u_TexId],v_texCoords).rgb;
    color = pow(color, vec3(2.2));

    vec3 ambient = 0.05 * color;

    vec3 lightDir = normalize(u_LightPos);
    vec3 normal = normalize(v_normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 light_atten_coff = u_LightIntensity / pow(length(u_LightPos - v_position), 2.0);
    vec3 diffuse = diff * light_atten_coff * color;

    vec3 viewDir = normalize(u_CameraPos - v_position);
    vec3 halfDir = normalize((lightDir + viewDir));
    float spec = pow(max(dot(halfDir, normal), 0.0), u_Ns);
    vec3 specular = u_Ks * light_atten_coff * spec;

    vec3 radiance = (ambient + diffuse + specular);
 
    vec3 phongColor = pow(radiance, vec3(1.0 / 2.2));
    return phongColor;
}

bool hit(vec3 position,vec3 direction,out vec3 hitbary)
{
    return true;
}

vec3 light_color(vec3 ray_dir,vec3 ray_point,vec3 normal,vec3 ks,vec3 kd,float ns,inout float s)
{
    vec3 color = vec3(0.0);
    for(int i = 0;i < u_LightNums;i++)
    {
        vec3 uv;
        uv.xy = Rand2(s);
        uv.y *= (1 - uv.x);
        uv.z = 1 - uv.x - uv.y;
        vec3 LV1 = vec3(m_Lights[i * 3].Vertex[0],m_Lights[i * 3].Vertex[1],m_Lights[i * 3].Vertex[2]);
        vec3 LV2 = vec3(m_Lights[i * 3 + 1].Vertex[0],m_Lights[i * 3 + 1].Vertex[1],m_Lights[i * 3 + 1].Vertex[2]);
        vec3 LV3 = vec3(m_Lights[i * 3 + 2].Vertex[0],m_Lights[i * 3 + 2].Vertex[1],m_Lights[i * 3 + 2].Vertex[2]);

        vec3 LE1 = vec3(m_Lights[i * 3].Le[0],m_Lights[i * 3].Le[1],m_Lights[i * 3].Le[2]);
        vec3 LE2 = vec3(m_Lights[i * 3 + 1].Le[0],m_Lights[i * 3 + 1].Le[1],m_Lights[i * 3 + 1].Le[2]);
        vec3 LE3 = vec3(m_Lights[i * 3 + 2].Le[0],m_Lights[i * 3 + 2].Le[1],m_Lights[i * 3 + 2].Le[2]);

        vec3 LV = LV1 * uv.x + LV2 * uv.y + LV3 * uv.z;
        vec3 LE = LE1 * uv.x + LE2 * uv.y + LE3 * uv.z;

        float distance_s = length(LV - ray_point) * length(LV - ray_point);
        float area = TriangleArea(LV1,LV2,LV3);
        float pdf = distance_s / area; 
        float pdf2 = max(dot(normalize(LV - ray_point),normalize(normal)),0.0); 

        vec3 halfDir = normalize((LV - ray_point + ray_dir));
        float spec = pow(max(dot(halfDir, normal), 0.0), ns);
        color += LE * pdf2 / pdf * (kd + ks * spec);
    }
    return color;
}

bool ray_tracing(vec3 position,vec3 direction,out vec3 color)
{
    vec3 albedo[MAXBOUNCE];
    for(int i = 0;i < u_Bounce;i++)
    {

    }

    return true;
}

void main()
{
    float s = InitRand(gl_FragCoord.xy);
    // vec4 color = vec4(BlinnPhong(),1.0);
    vec4 color = vec4(light_color(u_CameraPos - v_position,v_position,v_normal,u_Ks,u_Kd,u_Ns,s),1.0);
    o_color = color;
    o_Entity = u_Entity;
    // o_Entity = int(m_Lights[0].Vertex[1]);
}
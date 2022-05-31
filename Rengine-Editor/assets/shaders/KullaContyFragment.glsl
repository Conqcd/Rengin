#version 450

uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;
uniform vec3 u_LightRadiance;
uniform vec3 u_LightDir;

uniform sampler2D u_AlbedoMap;
uniform float u_Metallic;
uniform float u_Roughness;
uniform sampler2D u_BRDFLut;
uniform sampler2D u_EavgLut;
uniform samplerCube u_CubeTexture;

uniform int u_Entity;

in vec2 v_TextureCoord;
in vec3 v_FragPos;
in vec3 v_Normal;

const float PI = 3.14159265359;

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_Entity;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
   // TODO: To calculate GGX NDF here
    
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    // TODO: To calculate Schlick G1 here
    
    return 1.0;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    // TODO: To calculate Smith G here

    return 1.0;
}

vec3 fresnelSchlick(vec3 F0, vec3 V, vec3 H)
{
    // TODO: To calculate Schlick F here
    return vec3(1.0);
}


//https://blog.selfshadow.com/publications/s2017-shading-course/imageworks/s2017_pbs_imageworks_slides_v2.pdf
vec3 AverageFresnel(vec3 r, vec3 g)
{
    return vec3(0.087237) + 0.0230685*g - 0.0864902*g*g + 0.0774594*g*g*g
           + 0.782654*r - 0.136432*r*r + 0.278708*r*r*r
           + 0.19744*g*r + 0.0360605*g*g*r - 0.2586*g*r*r;
}

vec3 MultiScatterBRDF(float NdotL, float NdotV)
{
    vec3 albedo = pow(texture2D(u_AlbedoMap, v_TextureCoord).rgb, vec3(2.2));

    vec3 E_o = texture2D(u_BRDFLut, vec2(NdotL, u_Roughness)).xyz;
    vec3 E_i = texture2D(u_BRDFLut, vec2(NdotV, u_Roughness)).xyz;

    vec3 E_avg = texture2D(u_EavgLut, vec2(0, u_Roughness)).xyz;
    // copper
    vec3 edgetint = vec3(0.827, 0.792, 0.678);
    vec3 F_avg = AverageFresnel(albedo, edgetint);
    
    // TODO: To calculate fms and missing energy here


    return vec3(1.0);
}

void main(void) {
    vec3 albedo = pow(texture2D(u_AlbedoMap, v_TextureCoord).rgb, vec3(2.2));

    vec3 N = normalize(v_Normal);
    vec3 V = normalize(u_CameraPos - v_FragPos);
    float NdotV = max(dot(N, V), 0.0);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, u_Metallic);

    vec3 Lo = vec3(0.0);

    // calculate per-light radiance
    vec3 L = normalize(u_LightDir);
    vec3 H = normalize(V + L);
    float distance = length(u_LightPos - v_FragPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = u_LightRadiance;

    float NDF = DistributionGGX(N, H, u_Roughness);   
    float G   = GeometrySmith(N, V, L, u_Roughness);
    vec3 F = fresnelSchlick(F0, V, H);
        
    vec3 numerator    = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 Fmicro = numerator / max(denominator, 0.001); 
    
    float NdotL = max(dot(N, L), 0.0);

    vec3 Fms = MultiScatterBRDF(NdotL, NdotV);
    vec3 BRDF = Fmicro + Fms;
    
    Lo += BRDF * radiance * NdotL;
    vec3 color = Lo;
    
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
    o_Color = vec4(color, 1.0);
    o_Entity = u_Entity;
}
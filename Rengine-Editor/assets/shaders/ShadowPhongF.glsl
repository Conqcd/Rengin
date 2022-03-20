#version 330

#define EPS 1e-2

// In 
in vec3 v_position;
in vec3 v_normal;
in vec2 v_texCoords;
in vec4 v_PositionFromLight;

// Out
layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_Entity;

uniform int u_Entity;
// uniform sampler2D u_texture[31];
uniform sampler2D u_texture;
uniform sampler2D u_ShadowMap;
uniform int u_TextureID;

//Camera
uniform vec3 u_CameraPos;

// Light
uniform vec3 u_LightPos;
uniform vec3 u_LightIntensity;

//Material
uniform vec3 u_Ks;
uniform vec3 u_Kd;
uniform vec3 u_Ka;
uniform float u_Ns;
uniform float u_Ni;

float unpack(vec4 rgbaDepth) {
    const vec4 bitShift = vec4(1.0, 1.0/256.0, 1.0/(256.0 * 256.0), 1.0/(256.0 * 256.0 * 256.0));
    return dot(rgbaDepth, bitShift);
}

vec3 BlinnPhong(sampler2D texture)
{
    vec3 color = texture2D(texture,v_texCoords).rgb;
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

float useShadowMap(sampler2D shadowMap, vec4 shadowCoord){
    float close = unpack(texture2D(shadowMap, vec2((shadowCoord.x + 1.0) * 0.5, (shadowCoord.y + 1.0) * 0.5)));
    float vis = ((shadowCoord.z + 1.0) * 0.5 - EPS) < close ? 1.0 : 0.0;
    return vis;
}

void main()
{    
    float visibility;
    visibility = useShadowMap(u_ShadowMap, v_PositionFromLight);
    // o_color = vec4(visibility,visibility,visibility,1);
    o_color = vec4(BlinnPhong(u_texture) * visibility,1.0);
    // o_color = vec4(BlinnPhong(),1.0);
    o_Entity = u_Entity;
}
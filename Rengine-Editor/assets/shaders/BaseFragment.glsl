#version 330

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texCoords;


layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_Entity;

uniform int u_Entity;
uniform sampler2D m_texture;

//Material
uniform vec3 u_Ks;
uniform vec3 u_Kd;
uniform vec3 u_Ka;
uniform vec3 u_LightPos;

vec3 BlinnPhone()
{
    vec3 color = texture2D(m_texture,v_texCoords).rgb;
    color = pow(color, vec3(2.2));

    vec3 ambient = u_ka * color;

    vec3 lightDir = normalize(uLightPos);
    vec3 normal = normalize(vNormal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 light_atten_coff = uLightIntensity / pow(length(uLightPos - vFragPos), 2.0);
    vec3 diffuse = diff * light_atten_coff * color;

    vec3 viewDir = normalize(uCameraPos - vFragPos);
    vec3 halfDir = normalize((lightDir + viewDir));
    float spec = pow(max(dot(halfDir, normal), 0.0), 32.0);
    vec3 specular = uKs * light_atten_coff * spec;

    vec3 radiance = (ambient + diffuse + specular);
    vec3 phongColor = pow(radiance, vec3(1.0 / 2.2));
    return phongColor;
}

void main()
{
    o_color = texture2D(m_texture,v_texCoords);
    o_Entity = u_Entity;
}
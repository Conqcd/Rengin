#version 330

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texCoords;

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_Entity;

uniform int u_Entity;
uniform sampler2D m_texture;

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

vec3 BlinnPhong()
{
    vec3 color = texture2D(m_texture,v_texCoords).rgb;
    color = pow(color, vec3(2.2));

    vec3 ambient = 0.05 * color;

    vec3 lightDir = normalize(u_LightPos);
    vec3 normal = normalize(v_normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 light_atten_coff = u_LightIntensity / pow(length(u_LightPos - v_position), 2.0);
    vec3 diffuse = diff * light_atten_coff * color;

    vec3 viewDir = normalize(u_CameraPos - v_position);
    vec3 halfDir = normalize((lightDir + viewDir));
    float spec = pow(max(dot(halfDir, normal), 0.0), 32.0);
    vec3 specular = u_Ks * light_atten_coff * spec;

    vec3 radiance = (ambient + diffuse + specular);
    // vec3 radiance = ambient + u_Ks * light_atten_coff;
    // vec3 radiance = specular;
    vec3 phongColor = pow(radiance, vec3(1.0 / 2.2));
    return phongColor;
}

void main()
{
    // o_color = texture2D(m_texture,v_texCoords);
    o_color = vec4(BlinnPhong(),1.0);
    o_Entity = u_Entity;
}
#version 450

in vec2 v_texCoords;

layout(location = 0) out vec4 o_color;
layout(location = 1) out vec4 o_color2;
// layout(location = 1) out int o_Entity;
// layout(location = 2) out vec4 o_Test;
// layout(location = 3) out vec4 o_Test2;

uniform sampler2D u_Screen;
uniform sampler2D u_DepthTex;
uniform sampler2D u_NormalTex;
uniform sampler2D u_PositionTex;
uniform sampler2D u_EntityTex;
uniform sampler2D u_LastScreen;

uniform vec2 u_WindowSize;
uniform int u_FilterSize;
uniform float u_sigmap;
uniform float u_sigmac;
uniform float u_sigman;
uniform float u_sigmad;



void main()
{
    vec2 tex = gl_FragCoord.xy / u_WindowSize;
    if(gl_FragCoord.z >= texture(u_Screen,tex).w)
        discard;
    vec3 color = vec3(0.0);
    vec3 bcolor = texture(u_Screen,tex).rgb;
    vec3 bnormal = texture(u_NormalTex,tex).rgb;
    vec3 bposition = texture(u_PositionTex,tex).rgb;
    float weight = 0,weight2 = 0,weight3 = 0;
    for(int i = -u_FilterSize / 2;i <= u_FilterSize / 2;i++)
    {
        for(int j = -u_FilterSize / 2; j <= u_FilterSize / 2; j++) {
            int ijsquare = i * i + j * j;

            vec2 texf = (gl_FragCoord.xy + vec2(i,j)) / u_WindowSize;
            vec3 ocolor = texture(u_Screen,texf).rgb; 
            vec3 onormal = texture(u_NormalTex,texf).rgb; 
            vec3 oposition = texture(u_PositionTex,texf).rgb; 
            float cijsquare = length(ocolor - bcolor) * length(ocolor - bcolor);
            float nij = acos(clamp(dot(bnormal,onormal),-1,1)); 
            float pij = 0;
            if(i != 0 || j != 0)
                pij = dot(bnormal,normalize(oposition - bposition));
            
            // weight2 += pij;

            float eWeight = exp(-float(ijsquare) / 2 / u_sigmap / u_sigmap - cijsquare / 2 / u_sigmac / u_sigmac 
            - nij * nij / 2 / u_sigman / u_sigman
            - pij * pij / 2 / u_sigmad / u_sigmad);
            color += vec3(eWeight) * ocolor;
            weight += eWeight;
        }
    }
    color /= vec3(weight);
    // o_color = vec4(color,texture(u_Screen,tex).w);
    // o_color = vec4(color,1,0);
    o_color = vec4(1,0);
    o_color2 = texture(u_Screen,tex);
    // o_Test = vec4(weight,weight2,weight3,1);
    // o_Test2 = vec4(bposition,1);
    // o_color = vec4(bcolor,1.0);
    // o_Entity = 1;
    // o_Entity = texture(u_EntityTex,tex).r;
}
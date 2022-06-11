#version 330

in vec2 v_texCoords;

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_Entity;

uniform sampler2D u_Screen;
uniform sampler2D u_DepthTex;
uniform sampler2D u_NormalTex;
uniform sampler2D u_PositionTex;
uniform sampler2D u_EntityTex;

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
    float weight = 0;
    for(int i = -u_FilterSize / 2;i <= u_FilterSize / 2;i++)
    {
        for(int j = -u_FilterSize / 2; j <= u_FilterSize / 2; j++) {
            int ijsquare = i * i + j * j;

            vec2 texf = (gl_FragCoord.xy + vec2(i,j)) / u_WindowSize;
            vec3 ocolor = texture(u_Screen,texf).rgb; 
            float cijsquare = length(ocolor - bcolor) * length(ocolor - bcolor);

            float eWeight = exp(-float(ijsquare) / 2 / u_sigmap / u_sigmap - cijsquare / 2 / u_sigmac / u_sigmac);
            // float eWeight = exp(-float(ijsquare) / 2 / u_sigmap / u_sigmap);
            // float eWeight = 1;
            color += vec3(eWeight) * ocolor;
            weight += eWeight;
        }
    }
    color /= vec3(weight);
    o_color = vec4(color,1.0);
    // o_color = vec4(bcolor,1.0);
    // o_color = vec4(1.0);
    // o_Entity = u_Entity;
}
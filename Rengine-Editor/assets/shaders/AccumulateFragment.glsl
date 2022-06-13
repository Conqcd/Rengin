#version 450

in vec2 v_texCoords;

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_Entity;
layout(location = 2) out vec4 o_Test;
layout(location = 3) out vec4 o_Test2;

uniform sampler2D u_Screen;
uniform sampler2D u_LastScreen;
uniform sampler2D u_EntityTex;
uniform sampler2D u_PositionTex;

uniform vec2 u_WindowSize;
uniform int u_FilterSize;
uniform float u_alpha;
uniform float u_k;
uniform mat4 u_LastView;
uniform mat4 u_LastProjection;



void main()
{
    vec2 tex = v_texCoords;
    if(gl_FragCoord.z >= texture(u_Screen,tex).w)
        discard;
    vec3 color = vec3(0.0);
    vec3 bcolor = texture(u_Screen,tex).rgb;
    vec4 bposition = vec4(texture(u_PositionTex,tex).rgb,1.0);
    vec4 ltex = u_LastProjection * u_LastView * bposition;
    vec3 lcolor = texture(u_LastScreen,ltex.xy).rgb;
    float weight = 0;
    vec3 var = vec3(0.0);
    for(int i = -u_FilterSize / 2;i <= u_FilterSize / 2;i++)
    {
        for(int j = -u_FilterSize / 2; j <= u_FilterSize / 2; j++) {
            vec2 texf = v_texCoords + vec2(i,j) / u_WindowSize;
            vec3 ocolor = texture(u_Screen,texf).rgb;

            color += ocolor;
            weight++;
        }
    }
    color /= vec3(weight);
    for(int i = -u_FilterSize / 2;i <= u_FilterSize / 2;i++)
    {
        for(int j = -u_FilterSize / 2; j <= u_FilterSize / 2; j++) {
            vec2 texf = v_texCoords + vec2(i,j) / u_WindowSize;
            vec3 ocolor = texture(u_Screen,texf).rgb;
            var += (ocolor - bcolor) * (ocolor - bcolor);
        }
    }
    var /= vec3(weight);
    var = sqrt(var);
    lcolor = clamp(lcolor,color - u_k * var,color + u_k * var);
    // o_color = vec4(u_alpha * bcolor + (1 - u_alpha) * lcolor,1.0);
    o_color = vec4(bcolor,1.0);
    // o_color = vec4(1.0);
    o_Test = vec4(lcolor,1);
    o_Test2 = vec4(ltex.xy,0,1);
    // o_Entity = 1;
    // o_Entity = texture(u_EntityTex,tex).r;
}
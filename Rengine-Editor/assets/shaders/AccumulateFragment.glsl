#version 450

in vec2 v_texCoords;

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_Entity;
layout(location = 2) out vec4 o_Test;
layout(location = 3) out vec4 o_Test2;

uniform sampler2D u_Screen;
uniform sampler2D u_LastScreen;
uniform sampler2D u_EntityTex;

uniform vec2 u_WindowSize;
uniform int u_FilterSize;
uniform float u_alpha;
uniform float u_k;
uniform mat4 u_LastView;
uniform mat4 u_LastProjection;



void main()
{
    vec2 tex = gl_FragCoord.xy / u_WindowSize;
    // if(gl_FragCoord.z >= texture(u_Screen,tex).w)
    //     discard;
    vec3 color = vec3(0.0);
    vec3 bcolor = texture(u_Screen,tex).rgb;
    for(int i = -u_FilterSize / 2;i <= u_FilterSize / 2;i++)
    {
        for(int j = -u_FilterSize / 2; j <= u_FilterSize / 2; j++) {
            vec2 texf = (gl_FragCoord.xy + vec2(i,j)) / u_WindowSize;
            vec3 ocolor = texture(u_Screen,texf).rgb; 

            color += ocolor;
        }
    }
    // o_color = vec4(bcolor,1.0);
    o_color = vec4(1.0);
    // o_Test = vec4(weight,weight2,weight3,1);
    // o_Test2 = vec4(bposition,1);
    // o_color = vec4(bcolor,1.0);
    // o_Entity = 1;
    // o_Entity = texture(u_EntityTex,tex).r;
}
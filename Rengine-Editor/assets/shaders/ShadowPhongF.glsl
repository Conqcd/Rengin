#version 330

#define EPS 1e-2

#define NUM_SAMPLES 80
#define BLOCKER_SEARCH_NUM_SAMPLES NUM_SAMPLES
#define PCF_NUM_SAMPLES NUM_SAMPLES
#define NUM_RINGS 10
#define LIGTHWIDHT 0.08

#define PI 3.141592653589793
#define PI2 6.283185307179586

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
uniform int u_RenderType;

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

vec2 poissonDisk[NUM_SAMPLES];

float unpack(vec4 rgbaDepth) 
{
    const vec4 bitShift = vec4(1.0, 1.0/256.0, 1.0/(256.0 * 256.0), 1.0/(256.0 * 256.0 * 256.0));
    return dot(rgbaDepth, bitShift);
}

float rand_1to1(highp float x ) 
{
    return fract(sin(x)*10000.0);
}

float rand_2to1(vec2 uv ) 
{
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float dt = dot( uv.xy, vec2( a,b ) ), sn = mod( dt, PI );
	return fract(sin(sn) * c);
}

void poissonDiskSamples( const in vec2 randomSeed ) 
{
    float ANGLE_STEP = PI2 * float( NUM_RINGS ) / float( NUM_SAMPLES );
    float INV_NUM_SAMPLES = 1.0 / float( NUM_SAMPLES );

    float angle = rand_2to1( randomSeed ) * PI2;
    float radius = INV_NUM_SAMPLES;
    float radiusStep = radius;

    for( int i = 0; i < NUM_SAMPLES; i ++ ) {
        poissonDisk[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
        radius += radiusStep;
        angle += ANGLE_STEP;
    }
}

void uniformDiskSamples( const in vec2 randomSeed ) 
{
    float randNum = rand_2to1(randomSeed);
    float sampleX = rand_1to1( randNum ) ;
    float sampleY = rand_1to1( sampleX ) ;

    float angle = sampleX * PI2;
    float radius = sqrt(sampleY);

    for( int i = 0; i < NUM_SAMPLES; i ++ ) {
        poissonDisk[i] = vec2( radius * cos(angle) , radius * sin(angle)  );

        sampleX = rand_1to1( sampleY ) ;
        sampleY = rand_1to1( sampleX ) ;

        angle = sampleX * PI2;
        radius = sqrt(sampleY);
    }
}

float findBlocker(sampler2D shadowMap,vec2 uv,float zReceiver) 
{
    float avgdep = 0.0;
    for(int i = 0; i < BLOCKER_SEARCH_NUM_SAMPLES; i++) 
    {
        avgdep += unpack(texture2D(shadowMap, uv + poissonDisk[i] * 0.005));
    }
    avgdep = avgdep / float(BLOCKER_SEARCH_NUM_SAMPLES);
    return avgdep;
}

float PCF(sampler2D shadowMap, vec4 coords) 
{
    vec3 position = vec3((coords.x + 1.0) * 0.5, (coords.y + 1.0) * 0.5,(coords.z + 1.0) * 0.5);
    poissonDiskSamples(position.xy);
    // uniformDiskSamples(position.xy);

    float ans = 0.0;
    for(int i = 0; i < PCF_NUM_SAMPLES; i++) {
        float close = unpack(texture2D(shadowMap, position.xy + poissonDisk[i] * 0.005));
        ans += close == 0.0 ? 1.0 : (position.z - EPS) < close? 1.0 : 0.0;
    }
    ans = ans / float(PCF_NUM_SAMPLES);
    return ans;
}

float PCSS(sampler2D shadowMap, vec4 coords)
{
    vec3 position = vec3((coords.x + 1.0) * 0.5, (coords.y + 1.0) * 0.5, (coords.z + 1.0) * 0.5);
    poissonDiskSamples(position.xy);

    // uniformDiskSamples(position.xy);
    // STEP 1: avgblocker depth
    float avgdepth = findBlocker(shadowMap, position.xy, position.z);
    // STEP 2: penumbra size
    float Wpenumbra = (position.z - avgdepth) * LIGTHWIDHT / avgdepth;
    // STEP 3: filtering

    float ans = 0.0;
    for(int i = 0; i < PCF_NUM_SAMPLES; i++) {
        float close = unpack(texture2D(shadowMap, position.xy + poissonDisk[i] * Wpenumbra));
        ans += close == 0.0 ? 1.0 : (position.z - EPS) < close ? 1.0 : 0.0;
    }
    ans = ans / float(PCF_NUM_SAMPLES);
    return ans;
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
    if(u_RenderType == 0)
        visibility = useShadowMap(u_ShadowMap, v_PositionFromLight);
    else if(u_RenderType == 1)
        visibility = PCF(u_ShadowMap, v_PositionFromLight);
    else
        visibility = PCSS(u_ShadowMap, v_PositionFromLight);

    o_color = vec4(BlinnPhong(u_texture) * visibility,1.0);
    o_Entity = u_Entity;
}
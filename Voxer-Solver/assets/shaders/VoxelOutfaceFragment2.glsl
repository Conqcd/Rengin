#version 450 core

uniform float u_maxvalue;
uniform float u_threshold;
uniform int u_TWidth;
uniform int u_THeight;
uniform int u_TDepth;

uniform mat4 u_ViewMatrix;
uniform mat3 u_NormalMatrix;

uniform float u_focalLength;
uniform float u_aspectRatio;
uniform vec2 u_viewportSize;
uniform vec3 u_rayOrigin;
uniform vec3 u_top;
uniform vec3 u_bottom;

uniform vec3 u_backgroundColor;
uniform vec3 u_materialColor;
uniform vec3 u_lightPosition;

uniform float u_stepLength;
uniform int u_Choose;

uniform int u_RenderMode;
uniform sampler3D u_volume;
uniform sampler3D u_Force;
uniform sampler3D u_Constraint;
uniform sampler3D u_ResultVolume;
// layout (binding = 0) uniform sampler3D u_volume;
// layout (binding = 1) uniform sampler3D u_Force;
// layout (binding = 2) uniform sampler3D u_Constraint;
// layout (binding = 3) uniform sampler3D u_ResultVolume;
uniform float u_gamma;
layout(location = 0) out vec4 o_color;
layout(location = 1) out ivec3 o_position;
layout(location = 2) out vec4 o_displacement;

struct Ray
{
    vec3 origin,direction;
};

struct AABB {
    vec3 top;
    vec3 bottom;
};

struct MapNodeC
{
    float data;
    vec3 color;
};

struct MapNodeA
{
    float data;
    float opacity;
};

uniform MapNodeC u_mapc[100];
uniform MapNodeA u_mapa[100];

uniform int u_nodecNum;
uniform int u_nodeaNum;

vec3 normal(vec3 position, float intensity)
{
    float d = u_stepLength;
    float dx = texture(u_volume, position + vec3(d,0,0)).r - intensity;
    float dy = texture(u_volume, position + vec3(0,d,0)).r - intensity;
    float dz = texture(u_volume, position + vec3(0,0,d)).r - intensity;
    return -normalize(u_NormalMatrix * vec3(dx, dy, dz));
}

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void ray_box_intersection(Ray ray, AABB box, out float t_0, out float t_1)
{
    vec3 direction_inv = 1.0 / ray.direction;
    vec3 t_top = direction_inv * (box.top - ray.origin);
    vec3 t_bottom = direction_inv * (box.bottom - ray.origin);
    vec3 t_min = min(t_top, t_bottom);
    vec2 t = max(t_min.xx, t_min.yz);
    t_0 = max(0.0, max(t.x, t.y));
    vec3 t_max = max(t_top, t_bottom);
    t = min(t_max.xx, t_max.yz);
    t_1 = min(t.x, t.y);
}

vec4 color_transfer(float intensity)
{
    vec4 color;
    float rate = 0.5;
    for(int i = u_nodeaNum - 2;i >= 0;i--)
    {
        if(u_mapa[i].data <= intensity)
        {
            float opacity = u_mapa[i].opacity;
            float forward = float(u_mapa[i].data), back = float(u_mapa[i + 1].data);
            rate = (intensity - forward) / (back - forward);
            opacity = opacity * (1 - rate) + rate * u_mapa[i + 1].opacity;
            color.a = opacity;
            break;
        }
    }
    for(int i = u_nodecNum - 2;i >= 0;i--)
    {
        if(u_mapc[i].data <= intensity)
        {
            vec3 color2 = u_mapc[i].color;
            float forward = u_mapc[i].data, back = u_mapc[i + 1].data;
            float rate = (intensity - forward) / (back - forward);
            color2 = color2 * (1 - rate) + rate * u_mapc[i + 1].color;
            color.rgb = color2;
            break;
        }
    }
    return color;
}

void main()
{
    vec3 ray_direction;
    ray_direction.xy = 2.0 * gl_FragCoord.xy / u_viewportSize - 1.0;
    ray_direction.x *= u_aspectRatio;
    ray_direction.z = -u_focalLength;
    ray_direction = (vec4(ray_direction, 0) * u_ViewMatrix).xyz;

    float t_0 = 0, t_1 = 0;
    Ray casting_ray = Ray(u_rayOrigin, ray_direction);
    AABB bounding_box = AABB(u_top, u_bottom);
    ray_box_intersection(casting_ray, bounding_box, t_0, t_1);

    vec3 ray_start = (u_rayOrigin + ray_direction * t_0 - u_bottom) / (u_top - u_bottom);
    vec3 ray_stop = (u_rayOrigin + ray_direction * t_1 - u_bottom) / (u_top - u_bottom);
    vec3 ray = ray_stop - ray_start;
    float ray_length = length(ray);
    vec3 step_vector = u_stepLength * ray / ray_length;

    vec3 position = ray_start;

    // o_color = vec4(ray_length);
    // discard;
    vec4 color = vec4(0.0);
    // float cnt = 0;
    o_position = ivec3(-1,-1,-1);
    o_color = vec4(1,1,1,1);
    if(u_RenderMode == 4)
    {
        vec4 result = vec4(0.0);
        while (ray_length > 0 && color.a < 1.0) {
            if(((1 << (int(texture(u_volume, position).r) - 1)) & u_Choose) == 0)
            {
                ray_length -= u_stepLength;
                position += step_vector;
                continue;
            }
            result.rgb = texture(u_ResultVolume, position).rgb;
            result.a = length(result.rgb);
            float intensity = result.a / u_maxvalue;
            vec4 c = color_transfer(intensity);
            c.a = 1.0;

            vec3 L = normalize(u_lightPosition - position);
            vec3 V = -normalize(ray);
            vec3 N = normal(position, intensity);
            vec3 H = normalize(L + V);

            float Ia = 0.1;
            float Id = 1.0 * max(0, dot(N, L));
            float Is = 8.0 * pow(max(0, dot(N, H)), 600);

            color.rgb = c.a * c.rgb + (1 - c.a) * color.a * (color.rgb + (Ia + Id) * u_materialColor * 0.1 + Is * vec3(0.1));
            color.a = c.a + (1 - c.a) * color.a;
            // color.a = 1.0;
            

            // o_position.r = int(position.r * float(u_TWidth - 1));
            // o_position.g = int(position.g * float(u_THeight - 1));
            // o_position.b = int(position.b * float(u_TDepth - 1));
            if(intensity > 0.0)
                break;
            ray_length -= u_stepLength;
            position += step_vector;
        }

        // color.rgb = color.a * color.rgb + (1 - color.a) * pow(u_backgroundColor, vec3(u_gamma)).rgb;
        // color.a = 1.0;
        color.rgb = pow(color.rgb, vec3(1.0 / u_gamma));
        o_displacement = result * 1000000000.0;
    }else
    {
        float eps = 1e-3;
        float step_length = u_stepLength;
        float ray_length2 = ray_length;
        while (ray_length > 0 && color.a < 1.0) {
            float intensity = texture(u_volume, position).r;
            // if(((1 << (int(intensity) - 1)) & u_Choose) == 0) 
            // {
            //     ray_length -= u_stepLength;
            //     position += step_vector;
            //     continue;
            // }
            vec4 c = color_transfer(intensity / u_maxvalue);
            // if(intensity > u_threshold * u_maxvalue)
            // if(intensity > 0.0)
            {
                vec3 L = normalize(u_lightPosition - position);
                vec3 V = -normalize(ray);
                vec3 N = normal(position, intensity);
                vec3 H = normalize(L + V);

                float Ia = 0.1;
                float Id = 1.0 * max(0, dot(N, L));
                float Is = 8.0 * pow(max(0, dot(N, H)), 600);

                color.rgb = c.a * c.rgb + (1 - c.a) * color.a * (color.rgb + (Ia + Id) * u_materialColor * 0.1 + Is * vec3(0.1));
                // color.rgb = vec3(1.0);
                color.a = c.a + (1 - c.a) * color.a;
                // color.a = 1.0;

                // o_position.r = int(position.r * float(u_TWidth - 1));
                // o_position.r = 1;
                // o_position.r = int(position.b * float(u_TWidth - 1));
                // if(ray.r <= 0.0)  o_position.r -= 1;
                // o_position.g = int(position.g * float(u_THeight - 1));
                // if(ray.g <= 0.0)  o_position.g -= 1;
                // o_position.b = int(position.b * float(u_TDepth - 1));
                // o_position.b = int(position.r * float(u_TDepth - 1));
                // if(ray.b <= 0.0)  o_position.b -= 1;
                // if(step_length < eps)
                // {
                //     // color.rgb = vec3(1.0);
                //     color.a = c.a + (1 - c.a) * color.a;
                //     o_position.r = int(position.r * float(u_TWidth) + 0.5);
                //     o_position.g = int(position.g * float(u_THeight - 1));
                //     o_position.b = int(position.b * float(u_TDepth - 1));
                //     break;
                // }    
                // else
                // {
                //     ray_length += step_length;
                //     position -= step_vector;
                //     step_length /= 2;
                //     step_vector = step_length * ray / ray_length2;
                // }
            }

            ray_length -= step_length;
            position += step_vector;
        }

        // color.rgb = color.a * color.rgb + (1 - color.a) * pow(u_backgroundColor, vec3(u_gamma)).rgb;
        // color.a = 1.0;
        // color.rgb = pow(color.rgb, vec3(1.0 / u_gamma));
        // if(u_RenderMode == 2 && length(texture(u_Force, position).rgb) != 0.0)
        //     color = vec4(0.0,1.0,0.0,1.0);
        // else if(u_RenderMode == 3 && length(texture(u_Constraint, position).rgb) != 0.0)
        //     color = vec4(0.0,0.0,1.0,1.0);
    } 
    color.rgb = color.a * color.rgb + (1 - color.a) * pow(u_backgroundColor, vec3(u_gamma)).rgb;
    color.a = 1.0;
    o_color.rgb = pow(color.rgb, vec3(1.0 / u_gamma));
    o_color.a = color.a;
    // o_color = color;
    // o_color = vec4(1.0);
}
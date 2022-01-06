#version 330 core

uniform float maxvalue;

uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;

uniform float focal_length;
uniform float aspect_ratio;
uniform vec2 viewport_size;
uniform vec3 ray_origin;
uniform vec3 top;
uniform vec3 bottom;

uniform vec3 background_color;
uniform vec3 material_color;
uniform vec3 light_position;

uniform float step_length;

//uniform sampler3D volume;
uniform usampler3D volume;

uniform float gamma;

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

uniform MapNodeC mapc[20];
uniform MapNodeA mapa[20];

uniform int nodec_num;
uniform int nodea_num;

vec3 normal(vec3 position, float intensity)
{
    float d = step_length;
    float dx = texture(volume, position + vec3(d,0,0)).r - intensity;
    float dy = texture(volume, position + vec3(0,d,0)).r - intensity;
    float dz = texture(volume, position + vec3(0,0,d)).r - intensity;
    return -normalize(NormalMatrix * vec3(dx, dy, dz));
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

//vec4 colour_transfer(int intensity)
vec4 colour_transfer(uint intensity)
{
    vec4 color;
    float rate=0.5;
    for(int i = nodea_num - 2;i >= 0;i--)
    {
        if(mapa[i].data <= intensity)
        {
            float opacity = mapa[i].opacity;
            float forward = float(mapa[i].data), back = float(mapa[i + 1].data);
            rate = (float(intensity) - forward) / (back - forward);
            opacity = opacity * (1 - rate) + rate * mapa[i + 1].opacity;
            color.a = opacity;
            break;
        }
    }
    for(int i = nodec_num - 2;i >= 0;i--)
    {
        if(mapc[i].data <= intensity)
        {
            vec3 color2 = mapc[i].color;
            float forward = mapc[i].data, back = mapc[i + 1].data;
            float rate = (float(intensity) - forward) / (back - forward);
            color2 = color2 * (1 - rate) + rate * mapc[i + 1].color;
            color.rgb = color2;
            break;
        }
    }
    return color;
}

void main()
{
    vec3 ray_direction;
    ray_direction.xy = 2.0 * gl_FragCoord.xy / viewport_size - 1.0;
    ray_direction.x *= aspect_ratio;
    ray_direction.z = -focal_length;
    ray_direction = (vec4(ray_direction, 0) * ViewMatrix).xyz;

    float t_0 = 0, t_1 = 0;
    Ray casting_ray = Ray(ray_origin, ray_direction);
    AABB bounding_box = AABB(top, bottom);
    ray_box_intersection(casting_ray, bounding_box, t_0, t_1);

    vec3 ray_start = (ray_origin + ray_direction * t_0 - bottom) / (top - bottom);
    vec3 ray_stop = (ray_origin + ray_direction * t_1 - bottom) / (top - bottom);
    vec3 ray = ray_stop - ray_start;
    float ray_length = length(ray);
    vec3 step_vector = step_length * ray / ray_length;
    
    vec3 position = ray_start;

    vec4 color = vec4(0.0);

    while (ray_length > 0 && color.a < 1.0) {
        uint intensity = texture(volume, position).r;
        //int intensity = int(texture(volume, position).r);
        if(intensity > 0u)
        {

            vec4 c = colour_transfer(intensity);

            vec3 L = normalize(light_position - position);
            vec3 V = -normalize(ray);
            vec3 N = normal(position, intensity);
            vec3 H = normalize(L + V);

            float Ia = 0.1;
            float Id = 1.0 * max(0, dot(N, L));
            float Is = 8.0 * pow(max(0, dot(N, H)), 600);

            color.rgb = c.a * c.rgb + (1 - c.a) * color.a * (color.rgb + (Ia + Id) * material_color*0.1 + Is * vec3(0.1));
            color.rgb= vec3(1.0);
            //color.a = c.a + (1 - c.a) * color.a;
            break;
        }
//        vec3 L = normalize(light_position - position);
//        vec3 V = -normalize(ray);
//        vec3 N = normal(position, intensity);
//        vec3 H = normalize(L + V);

//        float Ia = 0.1;
//        float Id = 1.0 * max(0, dot(N, L));
//        float Is = 8.0 * pow(max(0, dot(N, H)), 600);

//        color.rgb = c.a * c.rgb + (1 - c.a) * color.a * (color.rgb + (Ia + Id) * material_color*0.1 + Is * vec3(0.1));
//        color.a = c.a + (1 - c.a) * color.a;

        ray_length -= step_length;
        position += step_vector;
    }
    
//    color.rgb = color.a * color.rgb + (1 - color.a) * pow(background_color, vec3(gamma)).rgb;
//    color.a = 1.0;
    gl_FragColor.rgb = pow(color.rgb, vec3(1.0 / gamma));

//    gl_FragColor.a = color.a;
}

#version 330 core
smooth in vec2 ray_offset;
out vec4 frag_color;

uniform float time;

uniform mat4 global_to_local;
uniform mat4 local_to_global;
uniform vec4 obj_params;
uniform vec3 obj_color;
uniform int obj_type;
uniform vec3 camera_pos;
uniform vec3 camera_dir;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform float light_intensity;
uniform float max_dist;
uniform vec3 ambient_color;
uniform float ambient_intensity;
uniform vec3 fog_color;

#define PI 3.1415926535897932384626433832795
#define EPS 0.0001

float maxcomp(vec3 v) {
    return max(max(v.x, v.y), v.z);
}

vec3 transform(vec3 p, mat4 m) {
    return (m * vec4(p, 1.0)).xyz;
}

float sd_sphere(vec3 p, vec4 params) {
    p = transform(p, global_to_local);
    float r = params.x;
    return length(p) - r;
}

float sd_box(vec3 p, vec4 params) {
    p = transform(p, global_to_local);
    vec3 dim = params.xyz;
    float rounded = params.w;
    vec3 q = abs(p) - dim;
    return length(max(q, 0.0)) + min(maxcomp(q), 0.0) - rounded;
}

float intersect(float a, float b) {
    return max(a, b);
}

float subtract(float base, float sub) {
    return max(base, -sub);
}

float sd(int type, vec3 p, vec4 params) {
    p = mod(p + 2, 4.0) - 2;
    if (type == 0) {
        return sd_sphere(p, params);
    } else if (type == 1) {
        return sd_box(p, params);
    } else if (type == 2) {
        return intersect(sd_box(p, params), sd_sphere(p, params + 0.12));
    }
}

vec3 compute_normal(int type, vec3 p) {
    vec2 e = vec2(EPS, 0.0);
    float d = sd(type, p, obj_params);
    return normalize(d - vec3(
        sd(type, p - e.xyy, obj_params),
        sd(type, p - e.yxy, obj_params),
        sd(type, p - e.yyx, obj_params)
    ));
}

void main() {
    vec3 u = normalize(camera_dir + vec3(ray_offset, 0.0));
    vec3 ray = vec3(0.0);
    for (int i = 0; i < 500; i++) {
        if (length(ray) > max_dist) {
            frag_color = vec4(fog_color, 1.0);
            return;
        }
        vec3 p = camera_pos + ray;
        float dist = sd(obj_type, p, obj_params);
        if (dist < EPS) {
            vec3 normal = compute_normal(obj_type, p);
            vec3 light_dir = normalize(light_pos - p);
            vec3 diffuse = max(dot(normal, light_dir), 0.0) * light_color;
            vec3 obj_colorr = normal * 0.5 + 0.5;
            frag_color = vec4((ambient_color * ambient_intensity + light_intensity * diffuse) * obj_colorr.zyx, 1.0);
            // frag_color = vec4(obj_colorr.zyx, 1.0);
            return;
        }
        ray += u * dist;
    }
    frag_color = vec4(fog_color, 1.0);
}

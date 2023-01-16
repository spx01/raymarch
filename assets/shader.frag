#version 460 core

in vec2 ray_offset;
out vec4 frag_color;

#define PI 3.1415926538

vec3 camera_dir = vec3(0.0, 0.0, 1.0);
vec3 camera_pos = vec3(0.0, 0.0, -1.0);
vec3 fog_color = vec3(0.0, 0.0, 1.0);
vec2 fov = vec2(PI / 2.0, PI / 2.0);
vec3 obj_center = vec3(0.0, 0.0, 0.5);
vec3 box_dim = vec3(0.3, 0.3, 0.3);
int max_steps = 1000;

vec2 fov_scale = vec2(tan(fov.x / 2.0), tan(fov.y / 2.0));

float sdf_sphere(vec3 p, vec3 c, float r) {
    return length(p - c) - r;
}

float sdf_box(vec3 p, vec3 b) {
    vec3 d = abs(p) - b;
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

void main() {
    float threshold = 0.01;
    vec3 u = normalize(camera_dir + vec3(fov_scale * ray_offset, 0.0));
//    vec3 ray = u  * sdf_sphere(camera_pos, obj_center, 0.3);
    vec3 ray = u  * (sdf_box(camera_pos - obj_center, box_dim) - 0.1);
    for (int i = 0; i < max_steps; i++) {
//        float d = sdf_sphere(ray + camera_pos, obj_center, 0.3);
        float d = sdf_box(ray + camera_pos - obj_center, box_dim) - 0.1;
        if (d < threshold) {
            frag_color = vec4(0.0, 1.0, 0.0, 1.0);
            return;
        }
        ray += u * d;
    }
    frag_color = vec4(fog_color, 1.0);
}

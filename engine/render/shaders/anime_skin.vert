#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;

layout(location = 0) out vec3 frag_normal;
layout(location = 1) out vec2 frag_uv;
layout(location = 2) out vec3 frag_world_pos;

layout(set = 0, binding = 0) uniform CameraUBO {
    mat4 view;
    mat4 proj;
    vec3 light_dir;
    float rim_intensity;
} camera;

layout(push_constant) uniform Push {
    mat4 model;
} push;

void main() {
    vec4 world_pos = push.model * vec4(in_position, 1.0);
    frag_world_pos = world_pos.xyz;
    frag_normal = mat3(push.model) * in_normal;
    frag_uv = in_uv;
    gl_Position = camera.proj * camera.view * world_pos;
}

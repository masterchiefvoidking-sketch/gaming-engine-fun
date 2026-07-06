#version 450

layout(location = 0) in vec3 frag_normal;
layout(location = 1) in vec2 frag_uv;
layout(location = 2) in vec3 frag_world_pos;

layout(location = 0) out vec4 out_color;

layout(set = 1, binding = 0) uniform SkinUBO {
    vec3 base_tone;
    vec3 shadow_tone;
    vec3 blush_color;
    float blush_intensity;
    float rim_intensity;
    vec3 rim_color;
    vec3 mood_tint;
    float wetness;
} skin;

layout(set = 0, binding = 0) uniform CameraUBO {
    mat4 view;
    mat4 proj;
    vec3 light_dir;
    float rim_pad;
} camera;

float toon_band(float ndotl) {
    if (ndotl < 0.25) return 0.0;
    if (ndotl < 0.55) return 0.5;
    if (ndotl < 0.8) return 0.85;
    return 1.0;
}

void main() {
    vec3 n = normalize(frag_normal);
    vec3 l = normalize(camera.light_dir);
    float ndotl = max(dot(n, l), 0.0);
    float band = toon_band(ndotl);

    vec3 albedo = mix(skin.shadow_tone, skin.base_tone, band) * skin.mood_tint;

    float blush_mask = smoothstep(0.35, 0.1, length(frag_uv - vec2(0.35, 0.55)));
    blush_mask += smoothstep(0.35, 0.1, length(frag_uv - vec2(0.65, 0.55)));
    albedo = mix(albedo, skin.blush_color, blush_mask * skin.blush_intensity);

    vec3 view_dir = normalize(vec3(0.0, 0.0, 1.0));
    float rim = pow(1.0 - max(dot(n, view_dir), 0.0), 3.0) * skin.rim_intensity;
    albedo += skin.rim_color * rim;

    albedo += vec3(skin.wetness * 0.15);

    out_color = vec4(albedo, 1.0);
}

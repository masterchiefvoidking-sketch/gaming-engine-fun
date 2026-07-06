#version 450

layout(location = 0) in vec2 frag_uv;

layout(location = 0) out vec4 out_color;

layout(set = 1, binding = 0) uniform EyeUBO {
    vec3 iris_color;
    float pupil_scale;
    float gloss_intensity;
    vec2 gaze_offset;
    float blink_amount;
} eye;

void main() {
    vec2 uv = frag_uv - 0.5 + eye.gaze_offset;
    float dist = length(uv);
    float iris = smoothstep(0.35, 0.25, dist);
    float pupil = smoothstep(0.15 * eye.pupil_scale, 0.1 * eye.pupil_scale, dist);

    vec3 color = mix(vec3(0.05), eye.iris_color, iris);
    color = mix(color, vec3(0.02), pupil);

    float highlight = smoothstep(0.08, 0.0, length(uv - vec2(0.12, 0.15)));
    highlight += smoothstep(0.04, 0.0, length(uv - vec2(-0.05, 0.08))) * 0.5;
    color += vec3(highlight * eye.gloss_intensity);

    float lid = smoothstep(0.0, 0.3, eye.blink_amount);
    color *= 1.0 - lid;

    out_color = vec4(color, 1.0);
}

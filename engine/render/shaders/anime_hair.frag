#version 450

layout(location = 0) in vec2 frag_uv;

layout(location = 0) out vec4 out_color;

layout(set = 1, binding = 0) uniform HairUBO {
    vec3 root_color;
    vec3 tip_color;
    vec3 highlight_color;
    float highlight_band_start;
    float highlight_band_end;
    float highlight_intensity;
} hair;

void main() {
    float t = frag_uv.y;
    vec3 base = mix(hair.root_color, hair.tip_color, t);

    float band = smoothstep(hair.highlight_band_start, hair.highlight_band_end, t) *
                 (1.0 - smoothstep(hair.highlight_band_end, hair.highlight_band_end + 0.1, t));
    base = mix(base, hair.highlight_color, band * hair.highlight_intensity);

    out_color = vec4(base, 1.0);
}

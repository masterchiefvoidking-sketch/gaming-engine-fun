#version 450

layout(location = 0) in vec3 inTangent;
layout(location = 1) in vec2 inUV;

layout(set = 1, binding = 0) uniform HairMaterialUBO {
    vec3 rootColor;
    vec3 tipColor;
    float roughness;
    float specularShift;
} hair;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 T = normalize(inTangent);
    vec3 hairColor = mix(hair.rootColor, hair.tipColor, inUV.y);
    float specular = pow(max(abs(T.z), 0.0), 32.0) * (1.0 - hair.roughness);
    vec3 color = hairColor + vec3(specular * hair.specularShift);
    outColor = vec4(color, 1.0);
}

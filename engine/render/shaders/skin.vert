#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(set = 0, binding = 0) uniform CameraUBO {
    mat4 view;
    mat4 projection;
} camera;

layout(set = 1, binding = 0) uniform SkinMaterialUBO {
    vec3 albedoTint;
    float roughness;
    float metallic;
    float subsurfaceRadius;
    float subsurfaceStrength;
    float specular;
} skin;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outUV;

void main() {
    vec4 worldPos = vec4(inPosition, 1.0);
    outWorldPos = worldPos.xyz;
    outNormal = inNormal;
    outUV = inUV;
    gl_Position = camera.projection * camera.view * worldPos;
}

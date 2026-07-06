#version 450

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(set = 0, binding = 0) uniform CameraUBO {
    mat4 view;
    mat4 projection;
    vec3 cameraPos;
} camera;

layout(set = 1, binding = 0) uniform SkinMaterialUBO {
    vec3 albedoTint;
    float roughness;
    float metallic;
    float subsurfaceRadius;
    float subsurfaceStrength;
    float specular;
} skin;

layout(location = 0) out vec4 outColor;

const vec3 LIGHT_DIR = normalize(vec3(0.3, -0.8, 0.4));

float distribution_ggx(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / max(3.14159265 * denom * denom, 0.0001);
}

void main() {
    vec3 N = normalize(inNormal);
    vec3 V = normalize(camera.cameraPos - inWorldPos);
    vec3 L = LIGHT_DIR;
    vec3 H = normalize(V + L);

    vec3 albedo = skin.albedoTint;
    float NdotL = max(dot(N, L), 0.0);

    float spec = distribution_ggx(N, H, skin.roughness) * skin.specular;
    vec3 diffuse = albedo * NdotL;

  // Subsurface scattering approximation
    float sss = pow(max(dot(-L, V), 0.0), skin.subsurfaceRadius) * skin.subsurfaceStrength;
    vec3 subsurface = albedo * sss * 0.35;

    vec3 color = diffuse + subsurface + vec3(spec);
    outColor = vec4(color, 1.0);
}

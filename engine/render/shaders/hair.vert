#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inTangent;
layout(location = 2) in vec2 inUV;

layout(set = 0, binding = 0) uniform CameraUBO {
    mat4 view;
    mat4 projection;
} camera;

layout(location = 0) out vec3 outTangent;
layout(location = 1) out vec2 outUV;

void main() {
    outTangent = inTangent;
    outUV = inUV;
    gl_Position = camera.projection * camera.view * vec4(inPosition, 1.0);
}

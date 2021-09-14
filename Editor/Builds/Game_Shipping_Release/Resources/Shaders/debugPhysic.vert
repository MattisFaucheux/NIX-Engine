#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform Camera {
    mat4 view;
    mat4 proj;
    vec3 position;
} camera;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 textCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPos;

void main() 
{
    gl_Position = camera.proj * camera.view * vec4(inPosition, 1.0);

    fragPos = inPosition;
    fragColor = vec3(1.0, 0.0, 0.0);
}
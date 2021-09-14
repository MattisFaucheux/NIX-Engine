#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform Camera {
    mat4 view;
    mat4 proj;
    vec3 position;
} camera;

layout(set = 2, binding = 0) uniform MatrixModel {
    mat4 model;
} modelMatrix;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 normal;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragPos;

void main() 
{
    vec3 lightPos = vec3(1.2, 1.0, 2.0);

    gl_Position = camera.proj * camera.view * modelMatrix.model * vec4(inPosition, 1.0);

    fragTexCoord = inTexCoord;
    fragPos = vec3(modelMatrix.model * vec4(inPosition, 1.0));
    normal = mat3(transpose(inverse(modelMatrix.model))) * inNormal;  
}
#version 460

#extension GL_ARB_separate_shader_objects : enable

#define MAX_LIGHTS 250

struct Light
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;
    vec4 direction;
            
	vec4 attenuation;

    int lightType;
    float innerCone;
	float outerCone;
<<<<<<< HEAD
=======
    bool isActive;
>>>>>>> 39a06a29ca31d6cb79318b0fdb273a7cde3fd7a4
    
};

layout(set = 0, binding = 0) uniform Camera {
    mat4 view;
    mat4 proj;
    vec3 position;
} camera;

layout(set = 0, binding = 1) uniform LightsArray {

   Light lights[MAX_LIGHTS];
};

layout(set = 0, binding = 2) uniform NbLights{
    int nbLights;
};

layout(set = 1, binding = 0) uniform sampler2D texSampler;
//layout(set = 1, binding = 1) uniform sampler2D normalMap;

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec2 inFragTexCoord;
layout(location = 2) in vec3 inFragPos;

layout(location = 0) out vec4 outColor;

vec3 ComputePointLight(int lightId);
vec3 ComputeDirectionalLight(int lightId);
vec3 ComputeSpotLight(int lightId);


void main() 
{    
    vec3 result = vec3(0, 0, 0);

    for (int lightId = 0 ; lightId < nbLights ; lightId++)
    {
<<<<<<< HEAD
=======
        if (lights[lightId].isActive == false)
        {
            continue;
        }

>>>>>>> 39a06a29ca31d6cb79318b0fdb273a7cde3fd7a4
        if (lights[lightId].lightType == 1) //Directional
        {
            result = ComputeDirectionalLight(lightId);
        }
        else if (lights[lightId].lightType == 2) // Point
        {
            result = ComputePointLight(lightId);
        }
        else if (lights[lightId].lightType == 3) // Spot
        {
            result = ComputeSpotLight(lightId);
        }
        
    } 
         outColor = vec4(result, 1.0) * texture(texSampler, inFragTexCoord);
<<<<<<< HEAD
        // outColor = texture(texSampler, inFragTexCoord);
    
=======
>>>>>>> 39a06a29ca31d6cb79318b0fdb273a7cde3fd7a4
}

vec3 ComputeDirectionalLight(int lightId)
{
    vec3 norm = normalize(inNormal);
	vec3 lightDir = normalize(vec3(-lights[lightId].direction));
	vec3 viewDir = normalize(camera.position - inFragPos);
	vec3 reflectDir = reflect(-lightDir, norm); 

	vec3 ambient = vec3(lights[lightId].ambient);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(lights[lightId].diffuse);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);
	vec3 specular = spec * vec3(lights[lightId].specular);  

    return (ambient + diffuse + specular);
}

vec3 ComputePointLight(int lightId)
{
    float distance = length(vec3(lights[lightId].position) - inFragPos);
	float attenuation = 1.0 / (lights[lightId].attenuation[0] + lights[lightId].attenuation[1] * distance + lights[lightId].attenuation[2] * (distance * distance));  

	vec3 norm = normalize(inNormal);
	vec3 lightDir = normalize(vec3(lights[lightId].position) - inFragPos); 
	vec3 viewDir = normalize(camera.position - inFragPos);
	vec3 reflectDir = reflect(-lightDir, norm); 

    vec3 ambient = vec3(lights[lightId].ambient);
	//vec3 ambient = material.ambient * light.ambient;

	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(lights[lightId].diffuse);
	//vec3 diffuse = diff * material.diffuse * light.diffuse;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);
	vec3 specular = spec * vec3(lights[lightId].specular);
	//vec3 specular = spec * material.specular * light.specular;  

     return (ambient + diffuse + specular) * attenuation;
}


vec3 ComputeSpotLight(int lightId)
{
   float distance = length(vec3(lights[lightId].position) - inFragPos);
	float attenuation = 1.0 / (lights[lightId].attenuation[0] + lights[lightId].attenuation[1] * distance + lights[lightId].attenuation[2] * (distance * distance));  

	vec3 norm = normalize(inNormal);
	vec3 lightDir = normalize(vec3(lights[lightId].position) - inFragPos);

	vec3 viewDir = normalize(camera.position - inFragPos);
<<<<<<< HEAD
	vec3 reflectDir = reflect(-lightDir, norm); 
=======
	vec3 reflectDir = reflect(-lightDir, norm);     
>>>>>>> 39a06a29ca31d6cb79318b0fdb273a7cde3fd7a4

	vec3 ambient = vec3(lights[lightId].ambient);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(lights[lightId].diffuse);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);
	vec3 specular = spec * vec3(lights[lightId].specular);  

	float theta     = dot(lightDir, normalize(vec3(-lights[lightId].direction)));
	float epsilon   = lights[lightId].innerCone - lights[lightId].outerCone;
	float intensity = clamp((theta - lights[lightId].outerCone) / epsilon, 0.0, 1.0);   

<<<<<<< HEAD
	return ((ambient + (diffuse * intensity) + (specular * intensity)) * attenuation );
=======
    return ((ambient + (diffuse * (1.0 - intensity)) + (specular * (1.0 - intensity))) * attenuation );
>>>>>>> 39a06a29ca31d6cb79318b0fdb273a7cde3fd7a4
}


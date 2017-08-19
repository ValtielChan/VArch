#version 430

// RENDER ==============================================================

struct Material {
    vec3 ambient;

    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

in vec3 FragPos;
in vec4 FragPosLightSpace;
in vec2 TexCoords;
in vec3 Normal;  
in vec3 Color;  
in vec4 DebugColor;
  
out vec4 color;
  
uniform sampler2D shadowMap;

uniform vec3 viewPos;
uniform Material material;

uniform int wireframe;

// LIGHTS ==============================================================

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

#define MAX_POINT_LIGHTS 8
uniform int nbPointLights;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

vec3 ComputeDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 overLum);
vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{
    vec3 overLum = vec3(wireframe);
    vec3 result = vec3(0, 0, 0);

    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
        
    result += ComputeDirLight(directionalLight, norm, viewDir, overLum);

    /*for (int i = 0; i < MAX_POINT_LIGHTS; i++)
        result += ComputePointLight(pointLights[i], norm, FragPos, viewDir);*/

    color = vec4(result, 1.0f);

} 

vec3 ComputeDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 overLum)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient  = light.ambient  * material.ambient;
    vec3 diffuse = (light.diffuse + overLum) * (diff * Color * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);  

    float shadow = ShadowCalculation(FragPosLightSpace);  
    return (ambient + (1.0 - shadow) * (diffuse + specular));
    //return FragPosLightSpace.xyz;
} 

vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0)
    ;
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + 
                 light.quadratic * (dist * dist));

    // combine results
    vec3 ambient  = light.ambient  * material.ambient * Color;
    vec3 diffuse = light.diffuse * (diff * Color * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);  

    /*ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;*/

    return (ambient + diffuse + specular);// * attenuation;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.005;

    //float shadow = currentDepth - bias > closestDepth ? 0.5 : 0.0;

    if(projCoords.z > 1.0) {
        float shadow = 0.0;
        return shadow;
    }

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
} 
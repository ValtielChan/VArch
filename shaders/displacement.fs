#version 430

struct Material {
    vec3 ambient;

    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;  
in vec4 DebugColor;
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight light;

uniform int wireframe;

void main()
{
    vec3 overLum = vec3(wireframe);

    // Ambient
    vec3 ambient = (light.ambient + overLum) * material.ambient;
    
    // Diffuse 
    vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightDir = normalize(-light.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    diff += wireframe;
    vec3 diffuse = (light.diffuse + overLum) * (diff * material.diffuse);
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    color = vec4(diffuse, 1.0f);

} 
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform PointLight pointLight;
uniform DirLight dirLight;
uniform vec3 viewPos;
uniform bool hasDiffuse;  
uniform bool hasNormal;   
uniform bool hasSpecular; 
uniform sampler2D texture_diffuse;  
uniform sampler2D texture_normal;   
uniform sampler2D texture_specular; 

void main()
{
    vec3 texDiffuse = hasDiffuse ? texture(texture_diffuse, TexCoords).rgb : vec3(1.0);
    vec3 texSpecular = hasSpecular ? texture(texture_specular, TexCoords).rgb : vec3(1.0);
    vec3 normal = hasNormal ? normalize(texture(texture_normal, TexCoords).rgb * 2.0 - 1.0) : normalize(Normal);

    // Ambient
    vec3 ambient = pointLight.ambient * (material.ambient * texDiffuse) +
                   dirLight.ambient * (material.ambient * texDiffuse);

    // Diffuse lighting from point light
    vec3 lightDir = normalize(pointLight.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 pointDiffuse = pointLight.diffuse * (diff * material.diffuse * texDiffuse);

    // Diffuse lighting from directional light
    vec3 dirDiffuse = max(dot(normal, normalize(-dirLight.direction)), 0.0) * dirLight.diffuse * (material.diffuse * texDiffuse);

    // Specular lighting from point light
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 pointSpecular = pointLight.specular * (spec * material.specular * texSpecular);

    // Specular lighting from directional light
    vec3 dirHalfwayDir = normalize(normalize(-dirLight.direction) + viewDir);
    float dirSpec = pow(max(dot(normal, dirHalfwayDir), 0.0), material.shininess);
    vec3 dirSpecular = dirLight.specular * (dirSpec * material.specular * texSpecular);

    // Combine results
    vec3 result = ambient + pointDiffuse + dirDiffuse + pointSpecular + dirSpecular;
    FragColor = vec4(result, 1.0);
}

#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform int numOfBands;

vec3 calcColorMix(float intensity, vec3 ambient, vec3 diffuse, vec3 specular) {
    vec3 ambientColor = ambient * light.ambient;
    vec3 diffuseColor = diffuse * light.diffuse * intensity;
    vec3 specularColor = specular * light.specular * intensity;

    return ambientColor + diffuseColor + specularColor;
}

void main() {
    vec3 lightDir = normalize(light.position - FragPos);
    float intensity = dot(lightDir, normalize(Normal));

    intensity = pow(intensity, material.shininess);

    intensity = floor(intensity * numOfBands) / numOfBands;

    vec4 color;

    // Calculate the color using the non-linear intensity
    color = vec4(calcColorMix(intensity, material.ambient, material.diffuse, material.specular), 1.0);

    FragColor = color;
}



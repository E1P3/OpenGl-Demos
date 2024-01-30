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

void main()
{

    vec3 lightDir = normalize(light.position - FragPos);
    float intensity = dot(lightDir,normalize(Normal));
    vec4 color;

    if(intensity > 0.95){
            color = vec4(material.ambient, 1.0);
    }
    else if(intensity > 0.5){
        color = vec4(material.ambient * 0.6, 1.0);
    }
    else if(intensity > 0.25){
        color = vec4(material.ambient * 0.4, 1.0);
    }
    else{
        color = vec4(material.ambient * 0.2, 1.0);
    }

    FragColor = color;
}

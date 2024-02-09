#version 410

layout(location = 0) out vec4 fragColor;

in vec3 gFragPos;
in vec3 gFacetNormal;
in vec2 gTexCoord;
in vec3 gTangentLightPos;
in vec3 gTangentViewPos;
in vec3 gTangentFragPos;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float height_scale;
};

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform Material material;

out vec4 FragColor;

vec3 getSpecular(vec3 normal , vec3 viewDir){
    vec3 halfwayDir = normalize(gTangentLightPos + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0), material.shininess);
    return material.specular * spec;
}

vec3 getAmbient(){
    return material.ambient;
}

vec3 getDiffuse(vec3 normal, vec3 color){
    vec3 lightDir = normalize(gTangentLightPos - gTangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    return diff * color;
}

void main() {

    vec3 normal = normalize(texture(texture_normal, gTexCoord).rgb * 2.0 - 1.0);
    vec3 viewDir = normalize(gTangentViewPos - gTangentFragPos);
    vec3 color = texture(texture_diffuse, gTexCoord).rgb;

    vec3 ambient = getAmbient();
    vec3 diffuse = getDiffuse(normal, color);
    vec3 specular = getSpecular(normal, viewDir);

    vec3 result = (ambient + diffuse + specular) * color;
    FragColor = vec4(ambient + diffuse + specular, 1.0);

}

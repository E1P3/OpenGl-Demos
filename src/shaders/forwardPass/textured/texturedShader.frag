#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float height_scale;  
};

uniform bool hasDiffuse;
uniform bool hasNormal;
uniform bool hasHeight;
uniform bool hasRoughness;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;
uniform sampler2D texture_roughness;

uniform Material material;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

out vec4 FragColor;

vec2 ParallaxMapping(vec2 TexCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 64;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * material.height_scale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = TexCoords;
    float currenttexture_heightValue = texture(texture_height, currentTexCoords).r;
      
    while(currentLayerDepth < currenttexture_heightValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get texture_height value at current texture coordinates
        currenttexture_heightValue = texture(texture_height, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currenttexture_heightValue - currentLayerDepth;
    float beforeDepth = texture(texture_height, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

vec3 getSpecular(vec3 normal , vec3 viewDir){
    vec3 halfwayDir = normalize(TangentLightPos + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0), material.shininess);
    return material.specular * spec;
}

vec3 getAmbient(){
    return material.ambient;
}

vec3 getDiffuse(vec3 normal, vec3 color){
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    return diff * color;
}

void main()
{
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    float height = hasHeight ? texture(texture_height, TexCoords).r : 0.0;
    vec2 texCoords = hasHeight ? ParallaxMapping(TexCoords, viewDir) : TexCoords;

    vec3 normal = hasNormal ? normalize(normalize(texture(texture_normal, texCoords).rgb) * 2.0 - 1.0) : Normal;
    vec3 color = hasDiffuse ? texture(texture_diffuse, texCoords).rgb : vec3(1.0);
    

    vec3 ambient = getAmbient();
    vec3 diffuse = getDiffuse(normal, color);
    vec3 specular = getSpecular(normal, viewDir);

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
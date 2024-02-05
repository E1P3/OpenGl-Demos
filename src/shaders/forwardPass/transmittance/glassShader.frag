#version 330 core

uniform samplerCube Cubemap;

in vec3 Reflect;
in vec3 RefractR;
in vec3 RefractG;
in vec3 RefractB;

in float Ratio;

out vec4 FragColor;

void main()
{
    vec3 refractColor, reflectColor;
    
    refractColor.r = vec3(texture(Cubemap, RefractR)).r;
    refractColor.g = vec3(texture(Cubemap, RefractG)).g;
    refractColor.b = vec3(texture(Cubemap, RefractB)).b;

    reflectColor = vec3(texture(Cubemap, Reflect));
    vec3 color = mix(refractColor, reflectColor, Ratio);
    FragColor = vec4(color, 1.0);
}
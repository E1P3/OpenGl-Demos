#version 330 core

uniform int hasDiffuse;
uniform int hasNormal;
uniform int hasHeight;
uniform int hasRoughness;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D heightMap;
uniform sampler2D roughnessMap;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

out vec4 FragColor;

void main()
{
    FragColor = vec4(color, 1.0);
}
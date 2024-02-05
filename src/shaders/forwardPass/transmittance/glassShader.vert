#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Reflect;
out vec3 RefractR;
out vec3 RefractG;
out vec3 RefractB;
out float Ratio;


uniform float eta;
uniform float fresnelPower;
uniform float chromaticOffset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

void main() {
    vec3 worldPos = vec3(model * vec4(aPos, 1.0)); 
    vec3 n = normalize(mat3(transpose(inverse(model))) * aNormal);
    vec3 i = normalize(camPos - worldPos);

    float etaR = eta - chromaticOffset;
    float etaG = eta;
    float etaB = eta + chromaticOffset;

    float F = ((1.0 - eta) * (1.0 - eta)) / ((1.0 + eta) * (1.0 + eta));
    Ratio = F + (1.0 - F) * pow((1.0 - dot(i, n)), fresnelPower);

    Reflect = reflect(-i, n);
    RefractR = refract(-i, n, etaR);
    RefractG = refract(-i, n, etaG);
    RefractB = refract(-i, n, etaB);

    gl_Position = projection * view * vec4(worldPos, 1.0);

}

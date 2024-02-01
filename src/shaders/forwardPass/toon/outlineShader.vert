#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float outlineThickness; // Uniform to control the scale of the outline

void main() {
    vec3 scaledPosition = aPos + (aNormal * outlineThickness);
    gl_Position = projection * view * model * vec4(scaledPosition, 1.0);
}
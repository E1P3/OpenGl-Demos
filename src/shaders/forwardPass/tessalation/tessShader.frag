#version 410

layout(location = 0) out vec4 fragColor;

in vec3 gFragPos;
in vec3 gFacetNormal;
in vec2 gTexCoord;

uniform vec3 lightPos;
uniform sampler2D texture_diffuse;

void main() {
    vec3 lightDir = normalize(lightPos - gFragPos);
    float diff = max(dot(gFacetNormal, lightDir), 0.0);
    vec3 color = texture(texture_diffuse, gTexCoord).rgb;
    fragColor = vec4(color * diff, 1.0);
}

#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;
uniform samplerCube cubemap;
uniform float eta; // Index of refraction
uniform float chromaticFactor; // Chromatic aberration factor

vec3 refractChromatic(vec3 I, vec3 N, float eta, float chromaticFactor) {
    // Separate refraction for R, G, B components
    vec3 refractedR = refract(I, N, eta + chromaticFactor);
    vec3 refractedG = refract(I, N, eta);
    vec3 refractedB = refract(I, N, eta - chromaticFactor);
    
    return vec3(refractedR.x, refractedG.y, refractedB.z);
}

float fresnel(vec3 I, vec3 N, float eta) {
    float cosi = clamp(dot(I, N), -1.0, 1.0);
    float sint = eta * sqrt(max(0.0, 1.0 - cosi * cosi));
    if (sint >= 1.0) {
        return 1.0; // Total internal reflection
    } else {
        float cost = sqrt(max(0.0, 1.0 - sint * sint));
        float Rs = ((eta * cosi) - cost) / ((eta * cosi) + cost);
        float Rp = (cosi - (eta * cost)) / (cosi + (eta * cost));
        return (Rs * Rs + Rp * Rp) / 2.0;
    }
}

void main() {
    vec3 I = normalize(FragPos - viewPos);
    vec3 N = normalize(Normal);
    float fresnelCoeff = fresnel(-I, N, eta);
    
    // Refraction with chromatic aberration
    vec3 refractedColor = texture(cubemap, refractChromatic(-I, N, eta, chromaticFactor)).rgb;
    
    // Reflection
    vec3 reflectedColor = texture(cubemap, reflect(-I, N)).rgb;
    
    // Mix reflection and refraction based on Fresnel coefficient
    vec3 color = mix(refractedColor, reflectedColor, fresnelCoeff);
    
    FragColor = vec4(color, 1.0);
}

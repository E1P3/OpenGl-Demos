#version 410

layout(vertices = 3) out;

in vec3 vPosition[];
in vec3 vNormal[];
in vec2 vTexCoord[];
in vec3 vTangent[];
in vec3 vBitangent[];
out vec3 tcPosition[];
out vec3 tcNormal[];
out vec2 tcTexCoord[];
out vec3 tcTangent[];
out vec3 tcBitangent[];

uniform vec3 viewPos;
uniform float innerTessLevel;
uniform float outerTessLevel;

float calculateTessellationLevel(vec3 viewPos, vec3 v0, vec3 v1, vec3 v2, float tessLevelFactor) {
    float dist0 = distance(viewPos, v0);
    float dist1 = distance(viewPos, v1);
    float dist2 = distance(viewPos, v2);

    float maxDist = max(dist0, max(dist1, dist2));

    float tessLevel = clamp(tessLevelFactor / maxDist, 1.0, 100.0);

    return tessLevel;
}

void main() {
    float inTess = calculateTessellationLevel(viewPos, vPosition[0], vPosition[1], vPosition[2], innerTessLevel);
    float outTess = calculateTessellationLevel(viewPos, vPosition[0], vPosition[1], vPosition[2], outerTessLevel);

    tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
    tcNormal[gl_InvocationID] = vNormal[gl_InvocationID];
    tcTexCoord[gl_InvocationID] = vTexCoord[gl_InvocationID];
    tcTangent[gl_InvocationID] = vTangent[gl_InvocationID];
    tcBitangent[gl_InvocationID] = vBitangent[gl_InvocationID];

    if (gl_InvocationID == 0) {
        gl_TessLevelInner[0] = inTess;
        gl_TessLevelInner[1] = inTess;
        gl_TessLevelOuter[0] = outTess;
        gl_TessLevelOuter[1] = outTess;
        gl_TessLevelOuter[2] = outTess;
        gl_TessLevelOuter[3] = outTess;
    }
}



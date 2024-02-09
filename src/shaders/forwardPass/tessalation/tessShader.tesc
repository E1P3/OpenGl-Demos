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

uniform float innerTessLevel;
uniform float outerTessLevel;

void main(){

    float inTess  = innerTessLevel;
    float outTess = outerTessLevel;

    tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
    tcNormal[gl_InvocationID]   = vNormal[gl_InvocationID];
    tcTexCoord[gl_InvocationID] = vTexCoord[gl_InvocationID];
    tcTangent[gl_InvocationID]  = vTangent[gl_InvocationID];
    tcBitangent[gl_InvocationID]= vBitangent[gl_InvocationID];

    if(gl_InvocationID == 0) {
        gl_TessLevelInner[0] = inTess;
        gl_TessLevelInner[1] = inTess;
        gl_TessLevelOuter[0] = outTess;
        gl_TessLevelOuter[1] = outTess;
        gl_TessLevelOuter[2] = outTess;
        gl_TessLevelOuter[3] = outTess;
    }
}
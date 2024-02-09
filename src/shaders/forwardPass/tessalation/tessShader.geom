#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 tePosition[3];
in vec2 teTexCoord[3];
in vec3 teNormal[3];
in vec3 teTangentLightPos[3];
in vec3 teTangentViewPos[3];
in vec3 teTangentFragPos[3];

out vec3 gFragPos;
out vec3 gFacetNormal;
out vec2 gTexCoord;
out vec3 gTangentLightPos;
out vec3 gTangentViewPos;
out vec3 gTangentFragPos;

void main() {

    gFacetNormal = teNormal[0];
    gTexCoord = teTexCoord[0];
    gFragPos = tePosition[0];
    gTangentLightPos = teTangentLightPos[0];
    gTangentViewPos = teTangentViewPos[0];
    gTangentFragPos = teTangentFragPos[0];
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    gFacetNormal = teNormal[1];
    gTexCoord = teTexCoord[1];
    gFragPos = tePosition[1];
    gTangentLightPos = teTangentLightPos[1];
    gTangentViewPos = teTangentViewPos[1];
    gl_Position = gl_in[1].gl_Position; EmitVertex();

    gFacetNormal = teNormal[2];
    gTexCoord = teTexCoord[2];
    gFragPos = tePosition[2];
    gTangentLightPos = teTangentLightPos[2];
    gTangentViewPos = teTangentViewPos[2];
    gl_Position = gl_in[2].gl_Position; EmitVertex();

    EndPrimitive();
}
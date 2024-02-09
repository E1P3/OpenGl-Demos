#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 tePosition[3];
in vec2 teTexCoord[3];
in vec3 teNormal[3];
out vec3 gFragPos;
out vec3 gFacetNormal;
out vec2 gTexCoord;

void main() {

    gFacetNormal = teNormal[0];
    gTexCoord = teTexCoord[0];
    gFragPos = tePosition[0];
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    gFacetNormal = teNormal[1];
    gTexCoord = teTexCoord[1];
    gFragPos = tePosition[1];
    gl_Position = gl_in[1].gl_Position; EmitVertex();

    gFacetNormal = teNormal[2];
    gTexCoord = teTexCoord[2];
    gFragPos = tePosition[2];
    gl_Position = gl_in[2].gl_Position; EmitVertex();

    EndPrimitive();
}
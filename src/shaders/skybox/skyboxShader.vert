#version 330 core
layout (location = 0) in vec3 vertex_position;

out vec3 TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoord = vertex_position;
    vec4 pos = projection * view * vec4(vertex_position, 1.0);
    gl_Position = pos.xyww;
}  
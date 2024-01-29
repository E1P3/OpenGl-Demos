#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_uv;
layout (location = 3) in vec3 vertex_tangent;
layout (location = 4) in vec3 vertex_bitangent;
layout (location = 5) in ivec4 vertex_boneIds; 
layout (location = 6) in vec4 vertex_weights;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;

void main() {
    // Perform the model-view-projection matrix multiplication
    gl_Position = proj * view * model * vec4(vertex_position, 1.0);

    // Transform the normal to world space and pass it to the fragment shader
    Normal = mat3(transpose(inverse(model))) * vertex_normal;

    // Calculate the fragment's position in world space
    FragPos = vec3(model * vec4(vertex_position, 1.0));
}

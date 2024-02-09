#version 410

layout(triangles, equal_spacing, ccw) in;

in vec3 tcPosition[];
in vec3 tcNormal[];
in vec2 tcTexCoord[];
in vec3 tcTangent[];
in vec3 tcBitangent[];

out vec3 tePosition;
out vec2 teTexCoord;
out vec3 teNormal;
out vec3 teTangentLightPos;
out vec3 teTangentViewPos;
out vec3 teTangentFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform vec3 lightPos;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float height_scale;
};

uniform sampler2D texture_height;
uniform Material material;

void main(){
    vec3 p0 = gl_TessCoord.x * tcPosition[0];
    vec3 p1 = gl_TessCoord.y * tcPosition[1];
    vec3 p2 = gl_TessCoord.z * tcPosition[2];
    vec3 pos = p0 + p1 + p2;

    vec3 n0 = gl_TessCoord.x * tcNormal[0];
    vec3 n1 = gl_TessCoord.y * tcNormal[1];
    vec3 n2 = gl_TessCoord.z * tcNormal[2];
    vec3 normal = normalize(n0 + n1 + n2);

    vec2 tc0 = gl_TessCoord.x * tcTexCoord[0];
    vec2 tc1 = gl_TessCoord.y * tcTexCoord[1];
    vec2 tc2 = gl_TessCoord.z * tcTexCoord[2];  
    teTexCoord = tc0 + tc1 + tc2;

    float height = texture(texture_height, teTexCoord).x;
    pos += normal * (height * material.height_scale);
    tePosition = vec3(model * vec4(pos, 1.0));
    teNormal = mat3(transpose(inverse(model))) * normal;

    vec3 T = normalize(vec3(model * vec4(tcTangent[0], 0.0f)));
    vec3 B = normalize(vec3(model * vec4(tcBitangent[0], 0.0f)));
    vec3 N = normalize(vec3(model * vec4(tcNormal[0], 0.0f)));
    mat3 TBN = transpose(mat3(T, B, N));

    teTangentLightPos = TBN * lightPos;
    teTangentViewPos = TBN * viewPos;
    teTangentFragPos = TBN * pos;

    gl_Position = projection * view * model * vec4(pos, 1.0f);
}
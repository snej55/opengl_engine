#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 Normal;
out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    // for normal mapping
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

// basic camera transformations
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;

// for normal mapping
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;
    Normal = aNormal;

    // create TBN matrix
    vec3 T = normalize(normalMat * aTangent);
    vec3 N = normalize(normalMat * aNormal);
    // vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 B = normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos = TBN * viewPos;
    vs_out.TangentFragPos = TBN * vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

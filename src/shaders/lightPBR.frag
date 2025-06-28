#version 410 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

uniform vec3 albedo;

const float ambientStrength = 0.05;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(viewPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = albedo * diff;
    vec3 ambient = albedo * ambientStrength;

    FragColor = vec4(diffuse + ambient, 1.0);
}
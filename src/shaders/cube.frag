#version 410 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

uniform vec3 albedo;

void main()
{
    vec3 norm = normalize(Normal * 0.5 + 0.5);
    vec3 lightDir = normalize(viewPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    FragColor = vec4(norm * diff, 1.0);
}
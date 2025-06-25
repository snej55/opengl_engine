#version 410 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.5, 0.1, 1.0);
}
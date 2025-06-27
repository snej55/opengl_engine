#version 410 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() {

    vec3 fragColor = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(pow(fragColor, vec3(1.0/2.2)), 1.0);
}
// Post processing shader that applies HDR and gamma correction.

#version 410 core

out vec4 FragColor;

in vec2 TexCoords;

// sample from screen framebuffer
uniform sampler2D screenTexture;

const float gamma = 2.2;

void main()
{
    // screenTexture is hdr framebuffer
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;

    // reinhard tone mapping
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));

    // gamma correction
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D uColorTexture;
uniform sampler2D uNormalTexture;
uniform sampler2D uDepthTexture;

void main() {
    vec4 color = texture(uColorTexture, TexCoord);
    // FragColor = vec4(vec3(color.r), 1.0);
    FragColor = color;
}

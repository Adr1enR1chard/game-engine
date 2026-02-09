#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D uTexture;
uniform vec4 uChannels = vec4(1, 1, 1, 1); // (1, 0, 0, 0) to sample from red channel, (0, 1, 0, 0) for green, etc. 

void main() {
    vec4 color = texture(uTexture, TexCoord);
    FragColor = color * uChannels;
}

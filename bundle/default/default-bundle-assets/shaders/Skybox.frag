#version 330 core
out vec4 FragColor;

in vec3 vUV;

uniform samplerCube colorMap;

void main() {
    FragColor = texture(colorMap, vUV);
}
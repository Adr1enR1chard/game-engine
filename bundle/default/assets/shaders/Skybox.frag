#version 330 core
out vec4 FragColor;

in vec3 vUV;

uniform samplerCube skybox;

void main() {
    FragColor = texture(skybox, vUV);
}
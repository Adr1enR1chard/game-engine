#version 330 core
in vec2 UV;

uniform sampler2D texture0;
uniform sampler2D faceTexture;

void main() {
    vec3 color1 = texture(texture0, UV).rgb;
    vec3 color2 = texture(faceTexture, UV).rgb;
    gl_FragColor = vec4(mix(color1, color2, 0.5), 1.0);
}
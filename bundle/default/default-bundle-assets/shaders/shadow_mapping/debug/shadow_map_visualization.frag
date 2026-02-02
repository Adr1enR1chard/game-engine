#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D uShadowMap;
uniform float uNearPlane;
uniform float uFarPlane;

float linearize_depth(float d, float zNear, float zFar) {
    float z_n = 2.0 * d - 1.0;
    return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}
void main() {
    float depth = texture(uShadowMap, TexCoord).r;
    float linearDepth = linearize_depth(depth, uNearPlane, uFarPlane) / uFarPlane;
    FragColor = vec4(vec3(linearDepth), 1.0);
}

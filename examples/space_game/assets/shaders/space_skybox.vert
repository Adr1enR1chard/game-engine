#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 vWorldRay;

uniform mat4 view;
uniform mat4 projection;

void main() {
    // Remove translation from view matrix
    mat3 viewRot = mat3(view);

    // Convert cube direction to world space
    vWorldRay = transpose(viewRot) * aPos;

    vec4 clipPos = projection * vec4(aPos, 1.0);
    gl_Position = clipPos.xyww;
}

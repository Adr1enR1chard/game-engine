#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec2 vUV;
out vec3 vWorldPos;
out vec3 vWorldNormal;
out mat3 vTBN;
out vec4 vFragPosLightSpace;

// ------ WORLD UNIFORMS ------
// -- Set in the RenderSystem -
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 uDirLightSpaceMatrix;
// -----------------------------

void main() {
    vUV = aUV;
    vWorldPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    mat3 normalMat = transpose(inverse(mat3(model)));

    vec3 N = normalize(normalMat * aNormal);
    vec3 T = normalize(normalMat * aTangent);
    vec3 B_in = normalize(normalMat * aBitangent);

    // Orthonormalize T against N
    T = normalize(T - N * dot(N, T));

    // Handedness from provided bitangent
    float sign = (dot(cross(N, T), B_in) < 0.0) ? -1.0 : 1.0;
    vec3 B = cross(N, T) * sign;

    vWorldNormal = N;
    vTBN = mat3(T, B, N);

    vFragPosLightSpace = uDirLightSpaceMatrix * vec4(vWorldPos, 1.0);
}
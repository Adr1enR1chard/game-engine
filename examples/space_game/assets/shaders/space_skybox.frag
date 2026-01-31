#version 330 core

in vec3 vWorldRay;
out vec4 FragColor;

uniform float uSunSize;
uniform float uPlanetSize;

// Directional light (sun)
uniform vec3 uSunDirection;   // normalized
uniform vec3 uSunColor;       // HDR-friendly

// ------------------------------------------------------------
// Utility
// ------------------------------------------------------------
float saturate(float x) {
    return clamp(x, 0.0, 1.0);
}

// ------------------------------------------------------------
// Hash / noise (direct CG → GLSL translation)
// ------------------------------------------------------------
float hash(float n) {
    return fract(sin(n) * 1e4);
}

float hash(vec2 p) {
    return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) *
        (0.1 + abs(sin(p.y * 13.0 + p.x))));
}

float noise(float x) {
    float i = floor(x);
    float f = fract(x);
    float u = f * f * (3.0 - 2.0 * f);
    return mix(hash(i), hash(i + 1.0), u);
}

float noise(vec2 x) {
    vec2 i = floor(x);
    vec2 f = fract(x);

    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
        (c - a) * u.y * (1.0 - u.x) +
        (d - b) * u.x * u.y;
}

float noise(vec3 x) {
    const vec3 step = vec3(110.0, 241.0, 171.0);

    vec3 i = floor(x);
    vec3 f = fract(x);
    float n = dot(i, step);

    vec3 u = f * f * (3.0 - 2.0 * f);

    return mix(mix(mix(hash(n + dot(step, vec3(0, 0, 0))), hash(n + dot(step, vec3(1, 0, 0))), u.x), mix(hash(n + dot(step, vec3(0, 1, 0))), hash(n + dot(step, vec3(1, 1, 0))), u.x), u.y), mix(mix(hash(n + dot(step, vec3(0, 0, 1))), hash(n + dot(step, vec3(1, 0, 1))), u.x), mix(hash(n + dot(step, vec3(0, 1, 1))), hash(n + dot(step, vec3(1, 1, 1))), u.x), u.y), u.z);
}

// ------------------------------------------------------------
// Sun attenuation (Unity Procedural Skybox model)
// ------------------------------------------------------------
float calcSunAttenuation(vec3 lightDir, vec3 ray) {
    float SunSizeConvergence = 10.0;

    float eyeCos = -pow(saturate(dot(lightDir, ray)), SunSizeConvergence);

    const float MIE_G = -0.990;
    const float MIE_G2 = 0.9801;

    float temp = 1.0 + MIE_G2 - 2.0 * MIE_G * eyeCos;
    temp = pow(temp, pow(uSunSize, 0.65) * 10.0);
    temp = max(temp, 1.0e-4);

    temp = 1.5 * ((1.0 - MIE_G2) / (2.0 + MIE_G2)) *
        (1.0 + eyeCos * eyeCos) / temp;

    return temp;
}

// ------------------------------------------------------------
// Fragment
// ------------------------------------------------------------
void main() {
    vec3 ray = normalize(vWorldRay);

    // --- Sun ---
    vec3 sunDir = normalize(uSunDirection);
    float sunWeight = calcSunAttenuation(sunDir, ray);

    // --- Planet ---
    vec3 planetDir = normalize(sunDir + vec3(0.5));
    vec3 coronaDir = normalize(planetDir + sunDir * uPlanetSize * 0.03);

    float planetWeight = smoothstep(0.0, 0.001, 1.0 - smoothstep(0.0, uPlanetSize, length(planetDir - ray)));

    float planetCoronaWeight = saturate(2.0 * smoothstep(0.0, 0.015, 1.0 - smoothstep(0.0, uPlanetSize, length(coronaDir - ray))));

    planetCoronaWeight = saturate(planetCoronaWeight - planetWeight);

    // --- Stars ---
    float starsWeight = smoothstep(0.96, 1.0, noise(ray * 100.0));

    // --- Masking ---
    sunWeight = saturate(sunWeight - planetWeight);
    starsWeight = saturate(starsWeight - planetWeight);

    vec3 color = sunWeight * uSunColor +
        planetCoronaWeight * uSunColor +
        starsWeight;

    FragColor = vec4(color, 1.0);
}

#version 330 core

struct DirLight {
    vec3 direction;
    vec3 color;
    float ambient;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

struct Material {
    vec3 baseColor;
    float metallic;
    float roughness;
    float ao;
    sampler2D baseColorMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;
    sampler2D normalMap;
};

out vec4 FragColor;

in vec2 vUV;
in vec3 vWorldPos;
in vec3 vWorldNormal;
in mat3 vTBN;

// ------ MATERIAL UNIFORMS -------
// --- Require defaults value per -
uniform Material material;
// -------------------------------

// ------ LIGHT UNIFORMS ------
uniform DirLight dirLight;
uniform PointLight pointLights[32]; // Max 32 point lights
uniform int pointLightCount;
// -----------------------------

// ------ WORLD UNIFORM ------
uniform vec3 viewPos; // Set in the LightSystem
// ----------------------------
const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 CalcDirLight(DirLight light, vec3 N, vec3 V, vec3 baseColor, float metallic, float roughness, float ao) {
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L);

    vec3 radiance = light.color * light.intensity;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, baseColor, metallic);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;

    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);
    return (kD * baseColor / PI + specular) * radiance * NdotL;
}

vec3 CalcPointLight(PointLight light, vec3 N, vec3 worldPos, vec3 V, vec3 baseColor, float metallic, float roughness, float ao) {
    vec3 L = normalize(light.position - worldPos);
    vec3 H = normalize(V + L);

    float distance = length(light.position - worldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = light.color * light.intensity * attenuation;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, baseColor, metallic);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);
    return (kD * baseColor / PI + specular) * radiance * NdotL;
}

void main() {
    vec3 N = vWorldNormal;
    N = texture(material.normalMap, vUV).rgb;
    N = normalize(vTBN * (N * 2.0 - 1.0));
    vec3 V = normalize(viewPos - vWorldPos);

    vec3 baseColor = pow(texture(material.baseColorMap, vUV).rgb * material.baseColor, vec3(2.2));

    float metallic = material.metallic * texture(material.metallicMap, vUV).r;

    float roughness = material.roughness * texture(material.roughnessMap, vUV).r;

    float ao = material.ao * texture(material.aoMap, vUV).r;

    vec3 Lo = CalcDirLight(dirLight, N, V, baseColor, metallic, roughness, ao);
    for(int i = 0; i < pointLightCount; ++i) {
        Lo += CalcPointLight(pointLights[i], N, vWorldPos, V, baseColor, metallic, roughness, ao);
    }

    vec3 ambient = vec3(0.03) * baseColor * ao * dirLight.ambient;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}
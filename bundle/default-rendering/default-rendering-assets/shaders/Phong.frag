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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
};

out vec4 FragColor;

in vec2 vUV;
in vec3 vWorldPos;
in vec3 vWorldNormal;
in mat3 vTBN;
in vec4 vFragPosLightSpace;

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

// ------- SHADOW UNIFORMS ------
uniform sampler2D uShadowMap;
uniform float uBias;
// -------------------------------

float ShadowCalculation(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0)
        return 0.0;

    float closestDepth = texture(uShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z - uBias;

    return currentDepth > closestDepth ? 1.0 : 0.0;
}

vec3 CalcDirLight(DirLight light, vec3 N, vec3 V, vec3 ambient, vec3 diffuse, vec3 specular, float shininess, float shadow) {
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L);

    vec3 radiance = light.color * light.intensity;

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), shininess);

    vec3 ambientComp = ambient * radiance * light.ambient;
    vec3 diffuseComp = diffuse * diff * radiance;
    vec3 specularComp = specular * spec * radiance;

    return (ambientComp + (1 - shadow) * (diffuseComp + specularComp));
}

vec3 CalcPointLight(PointLight light, vec3 N, vec3 worldPos, vec3 V, vec3 ambient, vec3 diffuse, vec3 specular, float shininess) {
    vec3 L = normalize(light.position - worldPos);
    vec3 H = normalize(V + L);

    float distance = length(light.position - worldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = light.color * light.intensity * attenuation;

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), shininess);

    vec3 diffuseComp = diffuse * diff * radiance;
    vec3 specularComp = specular * spec * radiance;

    return (diffuseComp + specularComp);
}

void main() {
    vec3 nTS = texture(material.normalMap, vUV).xyz * 2.0 - 1.0;
    vec3 N = normalize(vTBN * nTS);
    vec3 V = normalize(viewPos - vWorldPos);

    vec3 ambient = texture(material.diffuseMap, vUV).rgb * material.ambient;
    vec3 diffuse = texture(material.diffuseMap, vUV).rgb * material.diffuse;
    vec3 specular = texture(material.specularMap, vUV).rgb * material.specular;
    float shininess = material.shininess;

    float shadow = ShadowCalculation(vFragPosLightSpace);

    vec3 result = CalcDirLight(dirLight, N, V, ambient, diffuse, specular, shininess, shadow);
    for(int i = 0; i < pointLightCount; ++i) {
        result += CalcPointLight(pointLights[i], N, vWorldPos, V, ambient, diffuse, specular, shininess);
    }

    FragColor = vec4(result, 1.0);
}
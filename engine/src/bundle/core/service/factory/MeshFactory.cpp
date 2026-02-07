#include <engine/bundle/core/service/factory/MeshFactory.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace engine
{

    const std::vector<VertexLayout> kCubeVertices = {
        // Back face (0, 0, -1)
        {{-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0, 0}, {1, 0, 0}, {0, 1, 0}},
        {{0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1, 0}, {1, 0, 0}, {0, 1, 0}},
        {{0.5f, 0.5f, -0.5f}, {0, 0, -1}, {1, 1}, {1, 0, 0}, {0, 1, 0}},
        {{-0.5f, 0.5f, -0.5f}, {0, 0, -1}, {0, 1}, {1, 0, 0}, {0, 1, 0}},

        // Front face (0, 0, 1)
        {{-0.5f, -0.5f, 0.5f}, {0, 0, 1}, {0, 0}, {1, 0, 0}, {0, 1, 0}},
        {{0.5f, -0.5f, 0.5f}, {0, 0, 1}, {1, 0}, {1, 0, 0}, {0, 1, 0}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 1}, {1, 1}, {1, 0, 0}, {0, 1, 0}},
        {{-0.5f, 0.5f, 0.5f}, {0, 0, 1}, {0, 1}, {1, 0, 0}, {0, 1, 0}},

        // Left face (-1, 0, 0)
        {{-0.5f, 0.5f, 0.5f}, {-1, 0, 0}, {1, 0}, {0, 1, 0}, {0, 0, -1}},
        {{-0.5f, 0.5f, -0.5f}, {-1, 0, 0}, {1, 1}, {0, 1, 0}, {0, 0, -1}},
        {{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {0, 1}, {0, 1, 0}, {0, 0, -1}},
        {{-0.5f, -0.5f, 0.5f}, {-1, 0, 0}, {0, 0}, {0, 1, 0}, {0, 0, -1}},

        // Right face (1, 0, 0)
        {{0.5f, 0.5f, 0.5f}, {1, 0, 0}, {1, 0}, {0, 1, 0}, {0, 0, -1}},
        {{0.5f, 0.5f, -0.5f}, {1, 0, 0}, {1, 1}, {0, 1, 0}, {0, 0, -1}},
        {{0.5f, -0.5f, -0.5f}, {1, 0, 0}, {0, 1}, {0, 1, 0}, {0, 0, -1}},
        {{0.5f, -0.5f, 0.5f}, {1, 0, 0}, {0, 0}, {0, 1, 0}, {0, 0, -1}},

        // Bottom face (0, -1, 0)
        {{-0.5f, -0.5f, -0.5f}, {0, -1, 0}, {0, 1}, {1, 0, 0}, {0, 0, 1}},
        {{0.5f, -0.5f, -0.5f}, {0, -1, 0}, {1, 1}, {1, 0, 0}, {0, 0, 1}},
        {{0.5f, -0.5f, 0.5f}, {0, -1, 0}, {1, 0}, {1, 0, 0}, {0, 0, 1}},
        {{-0.5f, -0.5f, 0.5f}, {0, -1, 0}, {0, 0}, {1, 0, 0}, {0, 0, 1}},

        // Top face (0, 1, 0)
        {{-0.5f, 0.5f, -0.5f}, {0, 1, 0}, {0, 1}, {1, 0, 0}, {0, 0, 1}},
        {{0.5f, 0.5f, -0.5f}, {0, 1, 0}, {1, 1}, {1, 0, 0}, {0, 0, 1}},
        {{0.5f, 0.5f, 0.5f}, {0, 1, 0}, {1, 0}, {1, 0, 0}, {0, 0, 1}},
        {{-0.5f, 0.5f, 0.5f}, {0, 1, 0}, {0, 0}, {1, 0, 0}, {0, 0, 1}},
    };

    const std::vector<Index> kCubeIndices = {
        // Back face (-Z)
        0, 2, 1, 0, 3, 2,

        // Front face (+Z)
        4, 5, 6, 4, 6, 7,

        // Left face (-X)
        9, 10, 8, 10, 11, 8,

        // Right face (+X)
        14, 13, 12, 15, 14, 12,

        // Bottom face (-Y)
        17, 18, 16, 18, 19, 16,

        // Top face (+Y)
        22, 21, 20, 23, 22, 20};

    MeshRef MeshFactory::Cube()
    {
        return m_renderer.allocateMesh({kCubeVertices, kCubeIndices, glm::mat4(1.0f)});
    }

    MeshRef MeshFactory::Sphere(float radius, unsigned int sectorCount, unsigned int stackCount)
    {
        std::vector<VertexLayout> vertices;
        std::vector<Index> indices;

        for (unsigned int i = 0; i <= stackCount; ++i)
        {
            float stackAngle = glm::pi<float>() / 2 - i * glm::pi<float>() / stackCount; // from pi/2 to -pi/2
            float xy = radius * cosf(stackAngle);                                        // r * cos(u)
            float z = radius * sinf(stackAngle);                                         // r * sin(u)

            for (unsigned int j = 0; j <= sectorCount; ++j)
            {
                float sectorAngle = j * 2 * glm::pi<float>() / sectorCount; // from 0 to 2pi

                float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
                float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)

                glm::vec3 position = glm::vec3(x, y, z);
                glm::vec3 normal = glm::normalize(position);
                glm::vec2 texCoords = glm::vec2(static_cast<float>(j) / sectorCount, static_cast<float>(i) / stackCount);

                // Tangent and Bitangent calculation
                glm::vec3 tangent;
                tangent.x = -radius * sinf(stackAngle) * cosf(sectorAngle);
                tangent.y = -radius * sinf(stackAngle) * sinf(sectorAngle);
                tangent.z = radius * cosf(stackAngle);
                tangent = glm::normalize(tangent);

                glm::vec3 bitangent = glm::cross(normal, tangent);

                vertices.push_back({{position.x, position.y, position.z}, {normal.x, normal.y, normal.z}, {texCoords.x, texCoords.y}, {tangent.x, tangent.y, tangent.z}, {bitangent.x, bitangent.y, bitangent.z}});
            }
        }

        for (unsigned int i = 0; i < stackCount; ++i)
        {
            unsigned int k1 = i * (sectorCount + 1); // beginning of current stack
            unsigned int k2 = k1 + sectorCount + 1;  // beginning of next stack

            for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
                if (i != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
        return m_renderer.allocateMesh({vertices, indices, glm::mat4(1.0f)});
    }

    MeshRef MeshFactory::Plane()
    {
        std::vector<VertexLayout> vertices = {
            // Positions          // Normals         // TexCoords  // Tangents        // Bitangents
            {{-0.5f, 0.0f, -0.5f}, {0, -1, 0}, {0, 0}, {1, 0, 0}, {0, 0, -1}},
            {{0.5f, 0.0f, -0.5f}, {0, -1, 0}, {1, 0}, {1, 0, 0}, {0, 0, -1}},
            {{0.5f, 0.0f, 0.5f}, {0, -1, 0}, {1, 1}, {1, 0, 0}, {0, 0, -1}},
            {{-0.5f, 0.0f, 0.5f}, {0, -1, 0}, {0, 1}, {1, 0, 0}, {0, 0, -1}},
        };

        std::vector<Index> indices = {
            0,
            1,
            2,
            0,
            2,
            3,
        };

        return m_renderer.allocateMesh({vertices, indices, glm::mat4(1.0f)});
    }
} // namespace engine

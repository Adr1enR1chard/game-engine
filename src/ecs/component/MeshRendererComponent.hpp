#ifndef __MESH_RENDERER_HPP__
#define __MESH_RENDERER_HPP__

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "ecs/component/Component.hpp"
#include "handle/MeshHandle.hpp"

class MeshRendererComponent : public Component
{
private:
    std::shared_ptr<MeshHandle> mesh;

public:
    MeshRendererComponent(MeshHandle *mesh = nullptr) : mesh(mesh) {}

    ~MeshRendererComponent() {}

    MeshRendererComponent *setMesh(std::shared_ptr<MeshHandle> newMesh)
    {
        mesh = newMesh;
        return this;
    }

    void render() const
    {
        if (mesh)
        {
            glBindVertexArray(mesh->VAO);
            glDrawArrays(GL_TRIANGLES, 0, mesh->getVertexCount());
        }
    }
};

#endif
#ifndef __MESH_RENDERER_HPP__
#define __MESH_RENDERER_HPP__

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "core/components/component.hpp"
#include "core/data/mesh.hpp"

class MeshRenderer : public Component
{
private:
    std::shared_ptr<Mesh> mesh;

public:
    MeshRenderer(Mesh *mesh = nullptr) : mesh(mesh) {}

    ~MeshRenderer() {}

    MeshRenderer *setMesh(std::shared_ptr<Mesh> newMesh)
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
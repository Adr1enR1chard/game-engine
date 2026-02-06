#pragma once

#include <engine/registry/World.hpp>
#include <string>

using namespace engine;

class EntityPanel
{
public:
    EntityPanel(World &world) : world(world) {}
    ~EntityPanel() = default;

    void draw();

    Entity getSelectedEntity() const { return m_selectedEntity; }
    void setSelectedEntity(Entity entity) { m_selectedEntity = entity; }

private:
    World &world;
    Entity m_selectedEntity = 0;
    char m_searchBuffer[256] = "";
    bool m_showDeleteConfirm = false;
    Entity m_entityToDelete = 0;
};
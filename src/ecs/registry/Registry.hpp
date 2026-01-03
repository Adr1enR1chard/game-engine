#pragma once

#include <unordered_map>
#include <vector>
#include <queue>

#include "ecs/entity/Entity.hpp"
#include "ecs/component/Component.hpp"

class Registry
{
private:
    int entityIndex = 0;
    std::queue<int> freeIndices;
    std::unordered_map<int, Entity *> entityMap;

    std::vector<Component *> components;

public:
    Registry() {}
    ~Registry()
    {
        for (auto &[index, entity] : entityMap)
        {
            delete entity;
        }
        entityMap.clear();
    }

    Entity *createEntity()
    {
        int index;
        if (!freeIndices.empty())
        {
            index = freeIndices.front();
            freeIndices.pop();
        }
        else
        {
            index = entityIndex++;
        }

        Entity *entity = new Entity(index);
        entityMap[index] = entity;
        return entity;
    }

    void destroyEntity(int index)
    {
        auto it = entityMap.find(index);
        if (it != entityMap.end())
        {
            delete it->second;
            entityMap.erase(it);
            freeIndices.push(index);
        }
    }
};
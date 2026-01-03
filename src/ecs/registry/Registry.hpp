#pragma once

#include <unordered_map>
#include <vector>
#include <queue>

#include "ecs/entity/Entity.hpp"
#include "ecs/component/Component.hpp"
#include "utils/types.hpp"

class Registry
{
private:
    int m_entityIndex = 0;
    std::queue<int> m_freeIndices;

    std::vector<std::unique_ptr<Component>> m_components;

public:
    Registry() = default;
    Registry(const Registry &) = delete;
    Registry &operator=(const Registry &) = delete;
    Registry(Registry &&) = default;
    Registry &operator=(Registry &&) = default;
    ~Registry() = default;

    Entity createEntity()
    {
        Entity entity;
        if (!m_freeIndices.empty())
        {
            int index = m_freeIndices.front();
            m_freeIndices.pop();
            entity = index;
        }
        else
        {
            entity = m_entityIndex;
            m_entityIndex++;
        }

        return entity;
    }

    template <ComponentType T, typename... Args>
    T &createComponent(Args &&...args)
    {
        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        T &ref = *ptr;
        m_components.emplace_back(std::move(ptr));
        return ref;
    }

    template <ComponentType T>
    std::vector<T *> getComponents() const
    {
        std::vector<T *> out;
        for (auto &c : m_components)
            if (auto *casted = dynamic_cast<T *>(c.get()))
                out.push_back(casted);
        return out;
    }
};
#ifndef __GAME_OBJECT_HPP__
#define __GAME_OBJECT_HPP__

#include "core/components/transform.hpp"
#include "core/utils/types.hpp"
#include <vector>

class GameObject
{
private:
    std::vector<Component *> components;

public:
    GameObject()
    {
        addComponent<Transform>();
    };

    ~GameObject()
    {
        for (Component *component : components)
        {
            delete component;
        }
        components.clear();
    };

    template <ComponentType T>
    T *addComponent()
    {
        T *component = new T();

        if (component->unique() && hasComponent<T>())
        {
            std::cout << "Component already exists in this GameObject." << std::endl;
            delete component;
            return nullptr;
        }
        components.push_back(component);

        return component;
    }

    template <ComponentType T>
    T &getComponent()
    {
        for (Component *component : components)
        {
            T *castedComponent = dynamic_cast<T *>(component);
            if (castedComponent)
            {
                return *castedComponent;
            }
        }
        throw std::runtime_error("Component not found");
    }

    template <ComponentType T>
    bool hasComponent() const
    {
        for (Component *component : components)
        {
            if (dynamic_cast<T *>(component))
            {
                return true;
            }
        }
        return false;
    }
};

#endif
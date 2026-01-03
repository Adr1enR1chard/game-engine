#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

class Entity
{
private:
    int index;

public:
    Entity(int index) : index(index) {};

    ~Entity() {};

    int getIndex() const { return index; }
};

#endif
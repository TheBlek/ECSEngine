#pragma once

#include "constants.hpp"
#include "packed_array.hpp"
#include "entity.hpp"

class IComponentArray {
public:
    virtual void OnEntityDeletion(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public PackedArray<T, MAX_ENTITIES>, public IComponentArray {
    void OnEntityDeletion(Entity entity) override {
        this->RemoveData(entity);
    }
};

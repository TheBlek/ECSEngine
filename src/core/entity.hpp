#pragma once

#include "constants.hpp"
#include <bitset>

struct Signature {
    std::bitset<MAX_COMPONENTS> components;

    void AddComponent(Component id);

    void RemoveComponent(Component id);

    void Reset();

    bool IsSufficientFor(Signature& gate);
private:
    void VerifyID(Component id);
};
